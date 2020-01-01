// Includes
// --------------------

#include "Engine/Graphics/Graphics.h"
#include "UIGeometry.h"

namespace eae6320
{
	namespace UI {

		// Init Geometry data
		eae6320::cResult UIGeometry::InitializeGeometryData(Graphics::VertexFormats::sUIObject i_vertexData[],
			 uint8_t i_indexData[], unsigned int i_indexCount, unsigned int i_vertexCount)
		{
			m_indexCount = i_indexCount;
			auto result = eae6320::Results::Success;

			// Create a vertex array object and make it active
			{
				constexpr GLsizei arrayCount = 1;
				glGenVertexArrays(arrayCount, &m_vertexArrayId);
				const auto errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					glBindVertexArray(m_vertexArrayId);
					const auto errorCode = glGetError();
					if (errorCode != GL_NO_ERROR)
					{
						result = eae6320::Results::Failure;
						EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
						eae6320::Logging::OutputError("OpenGL failed to bind a new vertex array: %s",
							reinterpret_cast<const char*>(gluErrorString(errorCode)));
						return result;
					}
				}
				else
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					eae6320::Logging::OutputError("OpenGL failed to get an unused vertex array ID: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
					return result;
				}
			}
			// Create a vertex buffer object and make it active
			{
				constexpr GLsizei bufferCount = 1;
				glGenBuffers(bufferCount, &m_vertexBufferId);
				const auto errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
					const auto errorCode = glGetError();
					if (errorCode != GL_NO_ERROR)
					{
						result = eae6320::Results::Failure;
						EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
						eae6320::Logging::OutputError("OpenGL failed to bind a new vertex buffer: %s",
							reinterpret_cast<const char*>(gluErrorString(errorCode)));
						return result;
					}
				}
				else
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					eae6320::Logging::OutputError("OpenGL failed to get an unused vertex buffer ID: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
					return result;
				}
			}
			// create a index buffer object
			{
				constexpr GLsizei bufferCount = 1;
				glGenBuffers(bufferCount, &m_indexBufferId);
				const auto errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
					const auto errorCode = glGetError();
					if (errorCode != GL_NO_ERROR)
					{
						result = eae6320::Results::Failure;
						EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
						eae6320::Logging::OutputError("OpenGL failed to bind a new index buffer: %s",
							reinterpret_cast<const char*>(gluErrorString(errorCode)));
						return result;
					}
				}
				else
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					eae6320::Logging::OutputError("OpenGL failed to get an unused index buffer ID: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
					return result;
				}
			}

			// Assign the data to the buffer
			{
				const auto bufferSize = i_vertexCount * sizeof(*i_vertexData);
				EAE6320_ASSERT(bufferSize < (uint64_t(1u) << (sizeof(GLsizeiptr) * 8)));
				glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(bufferSize), reinterpret_cast<GLvoid*>(i_vertexData),
					// In our class we won't ever read from the buffer
					GL_STATIC_DRAW);
				const auto errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					eae6320::Logging::OutputError("OpenGL failed to allocate the vertex buffer: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
					return result;
				}

				// index buffer
				const auto bufferSize1 = m_indexCount * sizeof(uint8_t);
				EAE6320_ASSERT(bufferSize1 < (uint64_t(1u) << (sizeof(GLsizeiptr) * 8)));
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(bufferSize1), reinterpret_cast<GLvoid*>(i_indexData),
					// In our class we won't ever read from the buffer
					GL_STATIC_DRAW);
				const auto errorCode1 = glGetError();
				if (errorCode1 != GL_NO_ERROR)
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode1)));
					eae6320::Logging::OutputError("OpenGL failed to allocate the index buffer: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode1)));
					return result;
				}

			}
			// Initialize vertex format
			{
				// The "stride" defines how large a single vertex is in the stream of data
				// (or, said another way, how far apart each position element is)
				const auto stride = static_cast<GLsizei>(sizeof(eae6320::Graphics::VertexFormats::sUIObject));

				{
					constexpr GLuint vertexElementLocation = 0;
					constexpr GLint elementCount = 3;
					constexpr GLboolean notNormalized = GL_FALSE;	// The given floats should be used as-is
					glVertexAttribPointer(vertexElementLocation, elementCount, GL_FLOAT, notNormalized, stride,
						reinterpret_cast<GLvoid*>(offsetof(eae6320::Graphics::VertexFormats::sUIObject, x)));
					const auto errorCode = glGetError();
					if (errorCode == GL_NO_ERROR)
					{
						glEnableVertexAttribArray(vertexElementLocation);
						const GLenum errorCode = glGetError();
						if (errorCode != GL_NO_ERROR)
						{
							result = eae6320::Results::Failure;
							EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
							eae6320::Logging::OutputError("OpenGL failed to enable the POSITION vertex attribute at location %u: %s",
								vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
							return result;
						}
					}
					else
					{
						result = eae6320::Results::Failure;
						EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
						eae6320::Logging::OutputError("OpenGL failed to set the POSITION vertex attribute at location %u: %s",
							vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
						return result;
					}
				}
				// for color
				{
					constexpr GLuint vertexElementLocation = 1;
					constexpr GLint elementCount = 4;
					constexpr GLboolean notNormalized = GL_TRUE;	// The given floats should be used as-is
					glVertexAttribPointer(vertexElementLocation, elementCount, GL_UNSIGNED_BYTE, notNormalized, stride,
						reinterpret_cast<GLvoid*>(offsetof(eae6320::Graphics::VertexFormats::sUIObject, r)));

					const auto errorCode = glGetError();
					if (errorCode == GL_NO_ERROR)
					{
						glEnableVertexAttribArray(vertexElementLocation);
						const GLenum errorCode = glGetError();
						if (errorCode != GL_NO_ERROR)
						{
							result = eae6320::Results::Failure;
							EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
							eae6320::Logging::OutputError("OpenGL failed to enable the COLOR vertex attribute at location %u: %s",
								vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
							return result;
						}
					}
					else
					{
						result = eae6320::Results::Failure;
						EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
						eae6320::Logging::OutputError("OpenGL failed to set the COLOR vertex attribute at location %u: %s",
							vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
						return result;
					}
				}
			}

			return result;
		}
		
		// Draw
		void UIGeometry::Draw_Geometry()
		{
			// Bind a specific vertex buffer to the device as a data source
			{
				EAE6320_ASSERT(m_vertexArrayId != 0);
				glBindVertexArray(m_vertexArrayId);
				EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
			}
			// Render triangles from the currently-bound vertex buffer
			{
				// The mode defines how to interpret multiple vertices as a single "primitive";
				// a triangle list is defined
				// (meaning that every primitive is a triangle and will be defined by three vertices)
				constexpr GLenum mode = GL_TRIANGLES;
				// As of this comment only a single triangle is drawn
				// (you will have to update this code in future assignments!)
				
				//// It's possible to start rendering primitives in the middle of the stream
				
				const GLvoid* const offset = 0;
			//	int d = sizeof(m_indexBufferId);
				glDrawElements(mode, static_cast<GLsizei>(m_indexCount), GL_UNSIGNED_BYTE, offset);
				EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
			}
		}

		// Clean
		eae6320::cResult UIGeometry::Clean_GeometryData()
		{
			auto result = Results::Success;

			// Geometry data
			if (m_vertexArrayId != 0)
			{
				// Make sure that the vertex array isn't bound
				{
					// Unbind the vertex array
					glBindVertexArray(0);
					const auto errorCode = glGetError();
					if (errorCode != GL_NO_ERROR)
					{
						if (result)
						{
							result = Results::Failure;
						}
						EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
						Logging::OutputError("OpenGL failed to unbind all vertex arrays before cleaning up geometry: %s",
							reinterpret_cast<const char*>(gluErrorString(errorCode)));
					}
				}
				constexpr GLsizei arrayCount = 1;
				glDeleteVertexArrays(arrayCount, &m_vertexArrayId);
				const auto errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					if (result)
					{
						result = Results::Failure;
					}
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					Logging::OutputError("OpenGL failed to delete the vertex array: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
				}
				m_vertexArrayId = 0;
			}
			if (m_vertexBufferId != 0)
			{
				constexpr GLsizei bufferCount = 1;
				glDeleteBuffers(bufferCount, &m_vertexBufferId);
				const auto errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					if (result)
					{
						result = Results::Failure;
					}
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					Logging::OutputError("OpenGL failed to delete the vertex buffer: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
				}
				m_vertexBufferId = 0;
			}
			if (m_indexBufferId != 0)
			{
				constexpr GLsizei bufferCount = 1;
				glDeleteBuffers(bufferCount, &m_indexBufferId);
				const auto errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					if (result)
					{
						result = Results::Failure;
					}
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					Logging::OutputError("OpenGL failed to delete the vertex buffer: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
				}
				m_indexBufferId = 0;
			}

			return result;
		}

	}
}