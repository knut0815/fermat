/*
 * Copyright (c) 2010-2011, NVIDIA Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of NVIDIA Corporation nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

namespace cugar {

// get a set of stratified samples
template <typename T, uint32 DIM>
void LHSampler::sample(
	const uint32	num_samples,
	Vector<T, DIM>*	samples)
{
	const T inv = T(1.0) / T(num_samples);

	for (uint32 i = 0; i < num_samples; ++i)
	{
		for (uint32 d = 0; d < DIM; ++d)
			samples[i][d] = (T(i) + m_random.next()) * inv;
	}

	for (uint32 d = 0; d < DIM; ++d)
	{
		for (uint32 i = 0; i < num_samples; i++)
		{
			const float r = m_random.next();
			const uint32 j = min(uint32(float(r) * (num_samples - i)) + i, num_samples - 1u);
			std::swap(samples[i][d], samples[j][d]);
		}
	}
}

// get a set of stratified samples
template <bool SOA, typename T>
void LHSampler::sample(
	const uint32	num_samples,
	const uint32	num_dims,
	T*				samples)
{
	const T inv = T(1.0) / T(num_samples);

	for (uint32 i = 0; i < num_samples; ++i)
	{
		for (uint32 d = 0; d < num_dims; ++d)
			samples[SOA ? i + d*num_samples : i * num_dims + d] = (T(i) + m_random.next()) * inv;
	}

	for (uint32 d = 0; d < num_dims; ++d)
	{
		for (uint32 i = 0; i < num_samples; i++)
		{
			const float r = m_random.next();
			const uint32 j = min(uint32(float(r) * (num_samples - i)) + i, num_samples - 1u);
			std::swap(samples[SOA ? i + d*num_samples : i * num_dims + d], samples[SOA ? j + d*num_samples : j * num_dims + d]);
		}
	}
}

} // namespace cugar
