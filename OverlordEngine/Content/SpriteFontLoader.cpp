#include "stdafx.h"
#include "SpriteFontLoader.h"

SpriteFont* SpriteFontLoader::LoadContent(const ContentLoadInfo& loadInfo)
{
	const auto pReader = new BinaryReader();
	pReader->Open(loadInfo.assetFullPath);

	if (!pReader->Exists())
	{
		Logger::LogError(L"Failed to read the assetFile!\nPath: \'{}\'", loadInfo.assetSubPath);
		return nullptr;
	}

	//See BMFont Documentation for Binary Layout

	//Parse the Identification bytes (B,M,F)
	//If Identification bytes doesn't match B|M|F,
	//Log Error (SpriteFontLoader::LoadContent > Not a valid .fnt font) &
	//return nullptr
	std::string id{};
	for (int i{}; i < 3; i++)
	{
		id += pReader->Read<char>();
	}

	if (id != "BMF")
	{
		Logger::LogError(L"file format is not bmf");
		return nullptr;
	}


	//Parse the version (version 3 required)
	//If version is < 3,
	//Log Error (SpriteFontLoader::LoadContent > Only .fnt version 3 is supported)
	//return nullptr
	char version = pReader->Read<char>();
	if (version < 3)
	{
		Logger::LogError(L"File format versio  is lower than 3");
		return nullptr;
	}



	//Valid .fnt file >> Start Parsing!
	//use this SpriteFontDesc to store all relevant information (used to initialize a SpriteFont object)
	SpriteFontDesc fontDesc{};
	char blockId{};
	int blockSize{};
	int bufPos{};

	//**********
	// BLOCK 0 *
	//**********
	//Retrieve the blockId and blockSize
	//Retrieve the FontSize [fontDesc.fontSize]
	//Move the binreader to the start of the FontName [BinaryReader::MoveBufferPosition(...) or you can set its position using BinaryReader::SetBufferPosition(...))
	//Retrieve the FontName [fontDesc.fontName]

	//block info and buffer pos
	blockId = pReader->Read<char>();
	blockSize = pReader->Read<int>();
	bufPos = pReader->GetBufferPosition();

	//get fontsize
	int16_t fontSize{ pReader->Read<int16_t>() };
	fontDesc.fontSize = fontSize;

	//get fontName
	pReader->SetBufferPosition(bufPos + 14);
	fontDesc.fontName = pReader->ReadNullString();




	//**********
	// BLOCK 1 *
	//**********
	//Retrieve the blockId and blockSize
	//Retrieve Texture Width & Height [fontDesc.textureWidth/textureHeight]
	//Retrieve PageCount
	//> if pagecount > 1
	//	> Log Error (Only one texture per font is allowed!)
	//Advance to Block2 (Move Reader)

	//block info and buffer pos
	blockId = pReader->Read<char>();
	blockSize = pReader->Read<int>();
	bufPos = pReader->GetBufferPosition();

	//x size
	pReader->SetBufferPosition(bufPos + 4);
	fontDesc.textureWidth = pReader->Read<int16_t>();

	//y size
	pReader->SetBufferPosition(bufPos + 6);
	fontDesc.textureHeight = pReader->Read<int16_t>();

	//pages
	pReader->SetBufferPosition(bufPos + 8);
	int16_t pages = pReader->Read<uint16_t>();
	if (pages > 1)
	{
		Logger::LogError(L"Font has too many pages (more than one)");
		return nullptr;
	}

	pReader->SetBufferPosition(bufPos + 15);



	//**********
	// BLOCK 2 *
	//**********
	//Retrieve the blockId and blockSize
	//Retrieve the PageName (BinaryReader::ReadNullString)
	//Construct the full path to the page texture file
	//	>> page texture should be stored next to the .fnt file, pageName contains the name of the texture file
	//	>> full texture path = asset parent_path of .fnt file (see loadInfo.assetFullPath > get parent_path) + pageName (filesystem::path::append)
	//	>> Load the texture (ContentManager::Load<TextureData>) & Store [fontDesc.pTexture]

	//block info and buffer pos
	blockId = pReader->Read<char>();
	blockSize = pReader->Read<int>();
	bufPos = pReader->GetBufferPosition();

	
	std::wstring filePath = loadInfo.assetFullPath;
	size_t lastSlashPos{};

	while(filePath.find('/', lastSlashPos + 1 ) != std::wstring::npos)
	{
		lastSlashPos = filePath.find('/', lastSlashPos + 1);
	} 

	filePath = filePath.substr(0, lastSlashPos + 1) + pReader->ReadNullString();
	fontDesc.pTexture = ContentManager::Load<TextureData>(filePath);
	


	//**********
	// BLOCK 3 *
	//**********
	//Retrieve the blockId and blockSize
	//Retrieve Character Count (see documentation)
	//Create loop for Character Count, and:
	//> Retrieve CharacterId (store Local) and cast to a 'wchar_t'
	//> Create instance of FontMetric (struct)
	//	> Set Character (CharacterId) [FontMetric::character]
	//	> Retrieve Xposition (store Local)
	//	> Retrieve Yposition (store Local)
	//	> Retrieve & Set Width [FontMetric::width]
	//	> Retrieve & Set Height [FontMetric::height]
	//	> Retrieve & Set OffsetX [FontMetric::offsetX]
	//	> Retrieve & Set OffsetY [FontMetric::offsetY]
	//	> Retrieve & Set AdvanceX [FontMetric::advanceX]
	//	> Retrieve & Set Page [FontMetric::page]
	//	> Retrieve Channel (BITFIELD!!!) 
	//		> See documentation for BitField meaning [FontMetrix::channel]
	//	> Calculate Texture Coordinates using Xposition, Yposition, fontDesc.TextureWidth & fontDesc.TextureHeight [FontMetric::texCoord]
	//
	//> Insert new FontMetric to the metrics [font.metrics] map
	//	> key = (wchar_t) charId
	//	> value = new FontMetric
	//(loop restarts till all metrics are parsed)

	
	//block info and buffer pos
	blockId = pReader->Read<char>();
	blockSize = pReader->Read<int>();
	bufPos = pReader->GetBufferPosition();

	const int charStructSize = 20;
	int nrChars = blockSize / charStructSize;
	

	for (int c{}; c < nrChars; c++)
	{
		pReader->SetBufferPosition(bufPos + c * charStructSize);

		uint32_t cId = pReader->Read<uint32_t>();
		wchar_t charId = wchar_t(cId);

		FontMetric fMetric{};
		fMetric.character = charId;
		uint16_t posX = pReader->Read<uint16_t>();
		uint16_t posY = pReader->Read<uint16_t>();
		fMetric.width = pReader->Read<uint16_t>();
		fMetric.height = pReader->Read<uint16_t>();
		fMetric.offsetX = pReader->Read<uint16_t>();
		fMetric.offsetY = pReader->Read<uint16_t>();
		fMetric.advanceX = pReader->Read<uint16_t>();
		fMetric.page = pReader->Read<char>();

		char channelBitField = pReader->Read<char>();
		
		if ((channelBitField & 1) != 0) fMetric.channel = 2; //blue = 2
		else if ((channelBitField & 2) != 0) fMetric.channel = 1; //green = 1 
		else if ((channelBitField & 4) != 0) fMetric.channel = 0; //red = 0
		else if ((channelBitField & 8) != 0) fMetric.channel = 3; //alpha = 3


		fMetric.texCoord = XMFLOAT2( float(posX) / fontDesc.textureWidth, float(posY) / fontDesc.textureHeight);

		fontDesc.metrics.insert({ charId, fMetric });
	}









	//Done!
	delete pReader;
	return new SpriteFont(fontDesc);
}

void SpriteFontLoader::Destroy(SpriteFont* objToDestroy)
{
	SafeDelete(objToDestroy);
}
