#pragma once

namespace MathHelper
{
	inline XMFLOAT3 QuaternionToEuler(XMFLOAT4 q)
	{
		XMFLOAT3 euler{};

		euler.y = atan2f(2.f * q.x * q.w + 2.f * q.y * q.z, 1.f - 2.f * (q.z * q.z + q.w * q.w));     // Yaw 
		euler.x = asinf(2.f * (q.x * q.z - q.w * q.y));                             // Pitch 
		euler.z = atan2f(2.f * q.x * q.y + 2.f * q.z * q.w, 1.f - 2.f * (q.y * q.y + q.z * q.z));      // Roll 

		return euler;
	}

	inline bool XMFloat4Equals(const XMFLOAT4& a, const XMFLOAT4& b)
	{
		return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
	}

	inline bool XMFloat3Equals(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}

	inline bool XMFloat2Equals(const XMFLOAT2& a, const XMFLOAT2& b)
	{
		return a.x == b.x && a.y == b.y;
	}

	inline float randF(float min, float max)
	{
		const float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		const float diff = max - min;
		const float r = random * diff;
		return min + r;
	}

	inline LONG BinaryClamp(LONG x)
	{
		if (x > 0) return 1;
		if (x < 0) return -1;
		return 0;
	}

	template<typename T>
	void Clamp(T& value, T hi, T lo)
	{
		if (value > hi)
			value = hi;

		if (value < lo)
			value = lo;
	}


	inline float hueToColor(float p, float q, float hue)
	{
		if (hue < 0.f) hue += 1.f;
		if (hue > 1.f) hue -= 1.f;
		if (hue * 6.f < 1.f) return p + (q - p) * 6 * hue;
		if (hue * 2.f < 1.f) return q;
		if (hue * 3.f < 2.f) return p + (q - p) * (2.f / 3.f - hue) * 6.f;

		return p;
	}


	//all parameter values must range from 0.f to 1.f
	inline XMFLOAT4 HSLtoRGB(float h, float s, float l, float a)
	{
		Clamp(h, 1.f, 0.f);
		Clamp(s, 1.f, 0.f);
		Clamp(l, 1.f, 0.f);

		float r{}, g{}, b{};

		if (s == 0.f)
		{
			r = g = b = l;
		}
		else
		{
			float q = l < 0.5f ? l * (1 + s) : l + s - l * s;
			float p = 2 * l - q;

			float tempR{ h + (1.f / 3.f) };
			float tempG{ h };
			float tempB{ h - (1.f / 3.f) };

			r = hueToColor(p, q, tempR);
			g = hueToColor(p, q, tempG);
			b = hueToColor(p, q, tempB);
		}


		return XMFLOAT4(r, g, b, a);
	}
}