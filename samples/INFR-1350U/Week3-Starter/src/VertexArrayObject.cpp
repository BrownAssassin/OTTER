#include "VertexArrayObject.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

VertexArrayObject::VertexArrayObject() :
	_indexBuffer(nullptr),
	_handle(0)
{
<<<<<<< HEAD
	glCreateVertexArrays(1, &_handle);
=======
	// TODO: implement
>>>>>>> master
}

VertexArrayObject::~VertexArrayObject()
{
<<<<<<< HEAD
	if (_handle != 0) {
		glDeleteVertexArrays(1, &_handle);
		_handle = 0;
	}
}

void VertexArrayObject::SetIndexBuffer(IndexBuffer* ibo) {
	// TODO: What if we already have a buffer? should we delete it? who owns the buffer?
	_indexBuffer = ibo;
	Bind();
	if (_indexBuffer != nullptr) _indexBuffer->Bind();
	else IndexBuffer::UnBind();
	UnBind();
=======
	// TODO: implement
}

void VertexArrayObject::SetIndexBuffer(IndexBuffer* ibo) {
	// TODO: implement
>>>>>>> master
}

void VertexArrayObject::AddVertexBuffer(VertexBuffer* buffer, const std::vector<BufferAttribute>& attributes)
{
<<<<<<< HEAD
	// TODO: Who should own this buffer now? Do we delete it when we destroy?
	VertexBufferBinding binding;
	binding.Buffer = buffer;
	binding.Attributes = attributes;
	_vertexBuffers.push_back(binding);

	Bind();
	buffer->Bind();
	for (const BufferAttribute& attrib : attributes) {
		glEnableVertexArrayAttrib(_handle, attrib.Slot);
		glVertexAttribPointer(attrib.Slot, attrib.Size, attrib.Type, attrib.Normalized, attrib.Stride, (void*)attrib.Offset);
	}
	UnBind();
=======
	// TODO: implement
>>>>>>> master
}

void VertexArrayObject::Bind() {
	glBindVertexArray(_handle);
}

void VertexArrayObject::UnBind() {
	glBindVertexArray(0);
}
