#include "UIGeometry.h"
#include "Engine/Graphics/sContext.h"
#include "Engine/Graphics/VertexFormats.h"
#include <d3d11.h>

namespace eae6320
{
	
		// Init Geometry data
		eae6320::cResult UI::UIGeometry::InitializeGeometryData(Graphics::VertexFormats::sUIObject i_vertexData[],
			uint8_t i_indexData[], unsigned int i_indexCount, unsigned int i_vertexCount)
		{
			m_indexCount = i_indexCount;
			auto result = eae6320::Results::Success;
			
			auto* const direct3dDevice = eae6320::Graphics::sContext::g_context.direct3dDevice;
			EAE6320_ASSERT(direct3dDevice);

			// Vertex Format
			{
				if (!(result = eae6320::Graphics::cVertexFormat::s_manager.Load(eae6320::Graphics::VertexTypes::_3dObject, m_vertexFormat,
					"data/Shaders/Vertex/vertexInputLayout_3dObject.shader")))
				{
					EAE6320_ASSERTF(false, "Can't initialize geometry without vertex format");
					return result;
				}
			}
			// Vertex Buffer
			{
				for (unsigned int i = 0; i < m_indexCount; i++)
				{
					if (i % 3 == 0)
					{
						std::swap(i_indexData[i + 1], i_indexData[i + 2]);
					}
				}


				// convert to 16 cz 8 is not supported
				uint16_t customIndexData[6];
				for (unsigned int i = 0; i < m_indexCount; i++)
				{
					customIndexData[i] = (uint16_t)i_indexData[i];
				}

				// Vertex
				D3D11_BUFFER_DESC bufferDescription{};
				{
					const auto bufferSize = (i_vertexCount) * sizeof(eae6320::Graphics::VertexFormats::sUIObject);
					EAE6320_ASSERT(bufferSize < (uint64_t(1u) << (sizeof(bufferDescription.ByteWidth) * 8)));
					bufferDescription.ByteWidth = static_cast<unsigned int>(bufferSize);
					bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;	// In our class the buffer will never change after it's been created
					bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
					bufferDescription.CPUAccessFlags = 0;	// No CPU access is necessary
					bufferDescription.MiscFlags = 0;
					bufferDescription.StructureByteStride = 0;	// Not used
				}
				
				D3D11_SUBRESOURCE_DATA initialData{};
				{
					initialData.pSysMem = i_vertexData;
					// (The other data members are ignored for non-texture buffers)
				}

				const auto d3dResult = direct3dDevice->CreateBuffer(&bufferDescription, &initialData, &m_vertexBuffer);
				if (FAILED(d3dResult))
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF(false, "3D object vertex buffer creation failed (HRESULT %#010x)", d3dResult);
					eae6320::Logging::OutputError("Direct3D failed to create a 3D object vertex buffer (HRESULT %#010x)", d3dResult);
					return result;
				}

				// Index Buffer creation
				D3D11_BUFFER_DESC bufferDescription_1{};
				{
					const auto bufferSize_1 = i_indexCount * sizeof(uint16_t);
					EAE6320_ASSERT(bufferSize_1 < (uint64_t(1u) << (sizeof(bufferDescription_1.ByteWidth) * 8)));
					bufferDescription_1.ByteWidth = static_cast<unsigned int>(bufferSize_1);
					bufferDescription_1.Usage = D3D11_USAGE_IMMUTABLE;	// In our class the buffer will never change after it's been created
					bufferDescription_1.BindFlags = D3D11_BIND_INDEX_BUFFER;
					bufferDescription_1.CPUAccessFlags = 0;	// No CPU access is necessary
					bufferDescription_1.MiscFlags = 0;
					bufferDescription_1.StructureByteStride = 0;	// Not used
				}
				D3D11_SUBRESOURCE_DATA initialData_1{};
				{
					initialData_1.pSysMem = customIndexData;
					// (The other data members are ignored for non-texture buffers)
				}
				// index buffer creation
				const auto d3dResult_1 = direct3dDevice->CreateBuffer(&bufferDescription_1, &initialData_1, &m_indexBuffer);
				if (FAILED(d3dResult_1))
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF(false, "3D object index buffer creation failed (HRESULT %#010x)", d3dResult_1);
					eae6320::Logging::OutputError("Direct3D failed to create a 3D object index buffer (HRESULT %#010x)", d3dResult_1);
					return result;
				}
			}
			return result;
		}
		

		// Draw
		void UI::UIGeometry::Draw_Geometry()
		{
			auto* const direct3dImmediateContext = Graphics::sContext::g_context.direct3dImmediateContext;
			EAE6320_ASSERT(direct3dImmediateContext);

			// Bind a specific vertex buffer to the device as a data source
			{
				EAE6320_ASSERT(m_vertexBuffer);
				constexpr unsigned int startingSlot = 0;
				constexpr unsigned int vertexBufferCount = 1;
				// The "stride" defines how large a single vertex is in the stream of data
				constexpr unsigned int bufferStride = sizeof(Graphics::VertexFormats::sUIObject);
				// It's possible to start streaming data in the middle of a vertex buffer
				constexpr unsigned int bufferOffset = 0;
				direct3dImmediateContext->IASetVertexBuffers(startingSlot, vertexBufferCount, &m_vertexBuffer, &bufferStride, &bufferOffset);
			}
			

			// Specify what kind of data the vertex buffer holds
			{
				// Bind the vertex format (which defines how to interpret a single vertex)
				{
					EAE6320_ASSERT(m_vertexFormat);
					auto* const vertexFormat = Graphics::cVertexFormat::s_manager.Get(m_vertexFormat);
					EAE6320_ASSERT(vertexFormat);
					vertexFormat->Bind();
				}

				// Bind index buffer
				{
					EAE6320_ASSERT(m_indexBuffer);
					constexpr DXGI_FORMAT indexFormat = DXGI_FORMAT_R16_UINT;
					// offset for starting the indices
					constexpr unsigned int offset = 0;
					direct3dImmediateContext->IASetIndexBuffer(m_indexBuffer, indexFormat, offset);
				}
				// Set the topology (which defines how to interpret multiple vertices as a single "primitive";
				// the vertex buffer was defined as a triangle list
				// (meaning that every primitive is a triangle and will be defined by three vertices)
				
				direct3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				constexpr unsigned int indexOfFirstIndexToUse = 0;
				constexpr unsigned int offsetToAddToEachIndex = 0;
				direct3dImmediateContext->DrawIndexed(static_cast<unsigned int>(m_indexCount), indexOfFirstIndexToUse, offsetToAddToEachIndex);
			}
			// Render triangles from the currently-bound vertex buffer
			{
				// As of this comment only a single triangle is drawn
				// (you will have to update this code in future assignments!)  did it
			
				
			}
		}

		// Clean
		eae6320::cResult UI::UIGeometry::Clean_GeometryData()
		{
			auto result = Results::Success;

			// Geometry data
			
			if (m_vertexBuffer)
			{
				m_vertexBuffer->Release();
				m_vertexBuffer = nullptr;
			}
			if (m_indexBuffer)
			{
				m_indexBuffer->Release();
				m_indexBuffer = nullptr;
			}
			if (m_vertexFormat)
			{
				const auto result_vertexFormat = Graphics::cVertexFormat::s_manager.Release(m_vertexFormat);
				if (!result_vertexFormat)
				{
					EAE6320_ASSERT(false);
					if (result)
					{
						result = result_vertexFormat;
					}
				}
			}

			return result;
		}

	
}