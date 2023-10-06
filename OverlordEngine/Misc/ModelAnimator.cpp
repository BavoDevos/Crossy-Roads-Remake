#include "stdafx.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter) :
	m_pMeshFilter{ pMeshFilter }
{
	SetAnimation(0);
}

void ModelAnimator::Update(const SceneContext& sceneContext)
{

	//We only update the transforms if the animation is running and the clip is set
	if (m_IsPlaying && m_ClipSet)
	{
		//1. 
		//Calculate the passedTicks (see the lab document)
		//auto passedTicks = ...
		//Make sure that the passedTicks stay between the m_CurrentClip.Duration bounds (fmod)r
		auto passedTicks = sceneContext.pGameTime->GetElapsed() * m_CurrentClip.ticksPerSecond * m_AnimationSpeed;
		passedTicks = fmod(passedTicks, m_CurrentClip.duration);

		//2. 
		//IF m_Reversed is true
		//	Subtract passedTicks from m_TickCount
		//	If m_TickCount is smaller than zero, add m_CurrentClip.Duration to m_TickCount
		//ELSE
		//	Add passedTicks to m_TickCount
		//	if m_TickCount is bigger than the clip duration, subtract the duration from m_TickCount
		if (m_Reversed)
		{
			m_TickCount -= passedTicks;
			if (m_TickCount < 0) m_TickCount += m_CurrentClip.duration;
		}
		else
		{
			m_TickCount += passedTicks;
			if (m_TickCount > m_CurrentClip.duration) m_TickCount -= m_CurrentClip.duration;
		}


		//3.
		//Find the enclosing keys
		AnimationKey keyA, keyB;
		//Iterate all the keys of the clip and find the following keys:
		//keyA > Closest Key with Tick before/smaller than m_TickCount
		//keyB > Closest Key with Tick after/bigger than m_TickCount

		keyA = m_CurrentClip.keys.at(0);
		for (const auto& key : m_CurrentClip.keys)
		{
			const auto& tempKey = key;
			if (tempKey.tick > m_TickCount)
				break;

			keyA = tempKey;
		}

		for (const auto& key : m_CurrentClip.keys)
		{
			if (key.tick > m_TickCount)
			{
				keyB = key;
				break;
			}
		}


		//4.
		//Interpolate between keys
		//Figure out the BlendFactor (See lab document)
		//Clear the m_Transforms vector
		//FOR every boneTransform in a key (So for every bone)
		//	Retrieve the transform from keyA (transformA)
		//	auto transformA = ...
		// 	Retrieve the transform from keyB (transformB)
		//	auto transformB = ...
		//	Decompose both transforms
		//	Lerp between all the transformations (Position, Scale, Rotation)
		//	Compose a transformation matrix with the lerp-results
		//	Add the resulting matrix to the m_Transforms vector

		float tickDiff = keyB.tick - keyA.tick;
		float blendFactor = (m_TickCount - keyA.tick) / tickDiff;


		//std::cout << blendFactor << std::endl;

		ASSERT_IF((blendFactor < 0 || blendFactor > 1), L"blendfactor is not between 0 and 1");

		m_Transforms.clear();
		
		XMVECTOR posA{};
		XMVECTOR rotA{};
		XMVECTOR scaleA{};
		XMVECTOR posB{};
		XMVECTOR rotB{};
		XMVECTOR scaleB{};
		XMVECTOR pos{};
		XMVECTOR rot{};
		XMVECTOR scale{};

		for (int i{}; i < keyA.boneTransforms.size(); ++i)
		{
			auto transformA = XMLoadFloat4x4(&keyA.boneTransforms[i]);
			auto transformB = XMLoadFloat4x4(&keyB.boneTransforms[i]);


			XMMatrixDecompose(&scaleA, &rotA, &posA, transformA);
			XMMatrixDecompose(&scaleB, &rotB, &posB, transformB);


			pos = XMVectorLerp(posA, posB, blendFactor);
			rot = XMQuaternionSlerp(rotA, rotB, blendFactor);
			scale = XMVectorLerp(scaleA, scaleB, blendFactor);

			XMMATRIX transform =  XMMatrixAffineTransformation(scale, XMVectorZero(), rot, pos);

			XMFLOAT4X4 finalTransform{};
			XMStoreFloat4x4(&finalTransform, transform);

			m_Transforms.push_back(finalTransform);
		}

	}
		return;
}

void ModelAnimator::SetAnimation(const std::wstring& clipName)
{
	TODO_W7_();
	//Set m_ClipSet to false
	//Iterate the m_AnimationClips vector and search for an AnimationClip with the given name (clipName)
	//If found,
	//	Call SetAnimation(Animation Clip) with the found clip
	//Else
	//	Call Reset
	//	Log a warning with an appropriate message

	m_ClipSet = false;
	for (AnimationClip clip : m_pMeshFilter->m_AnimationClips)
	{
		if (clipName == clip.name)
		{
			SetAnimation(clip);
			return;
		}
	}

	Reset();
	Logger::LogWarning(L"no clip with name {} could be found ", clipName);


}

void ModelAnimator::SetAnimation(UINT clipNumber)
{
	//Set m_ClipSet to false
	//Check if clipNumber is smaller than the actual m_AnimationClips vector size
	//If not,
		//	Call Reset
		//	Log a warning with an appropriate message
		//	return
	//else
		//	Retrieve the AnimationClip from the m_AnimationClips vector based on the given clipNumber
		//	Call SetAnimation(AnimationClip clip)

	m_ClipSet = false;
	if (clipNumber >= m_pMeshFilter->m_AnimationClips.size())
	{
		Reset(true);
		Logger::LogWarning(L"clip number exceeds the amount of available clips in the mesh filter in mesh {}", m_pMeshFilter->m_MeshName);
		return;
	}
	else
	{
		AnimationClip clip = m_pMeshFilter->m_AnimationClips[clipNumber];
		SetAnimation(clip);
	}
}

void ModelAnimator::SetAnimation(const AnimationClip& clip)
{
	//Set m_ClipSet to true
	//Set m_CurrentClip

	m_ClipSet = true;
	m_CurrentClip = clip;
	//Call Reset(false)
	Reset(false);
}

void ModelAnimator::Reset(bool pause)
{
	//If pause is true, set m_IsPlaying to false

	//Set m_TickCount to zero
	//Set m_AnimationSpeed to 1.0f

	//If m_ClipSet is true
	//	Retrieve the BoneTransform from the first Key from the current clip (m_CurrentClip)
	//	Refill the m_Transforms vector with the new BoneTransforms (have a look at vector::assign)
	//Else
	//	Create an IdentityMatrix 
	//	Refill the m_Transforms vector with this IdenityMatrix (Amount = BoneCount) (have a look at vector::assign)

	if (pause) m_IsPlaying = false;

	m_TickCount = 0;
	m_AnimationSpeed = 1.f;

	if (m_ClipSet == true)
	{
		m_Transforms = m_CurrentClip.keys.at(0).boneTransforms;
	}
	else
	{
		XMMATRIX identity = XMMatrixIdentity();
		XMFLOAT4X4 iMatrix{};
		XMStoreFloat4x4(&iMatrix, identity);
		std::fill(m_Transforms.begin(), m_Transforms.end(), iMatrix);
	}


}
