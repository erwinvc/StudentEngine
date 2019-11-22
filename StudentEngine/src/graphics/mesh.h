#pragma once

class Mesh {
protected:
	ManagedRef<VertexArray> m_vao;
	ManagedRef<IndexBuffer> m_ibo;

public:
	Mesh(ManagedRef<VertexArray> vao, ManagedRef<IndexBuffer> ibo) : m_vao(vao), m_ibo(ibo) {}
	virtual ~Mesh() {}

	void Bind() {
		m_vao->Bind();
		m_ibo->Bind();
	}

	void Unbind() {
		m_vao->Unbind();
		m_ibo->Unbind();
	}

	void DrawInstanced(int amount, uint mode = GL_TRIANGLES) {
		Bind();
		m_ibo->DrawInstanced(amount, mode);
	}

	void Draw(uint mode = GL_TRIANGLES) {
		Bind();
		m_ibo->Draw(mode);
	}

	void DrawCount(uint32 count, uint mode = GL_TRIANGLES) {
		Bind();
		m_ibo->Draw(count, mode);
	}

	AssetRef<Mesh> Copy() {
		return new Mesh(m_vao, m_ibo);
	}

	inline ManagedRef<VertexArray> GetVAO() { return m_vao; }
	inline ManagedRef<IndexBuffer> GetIBO() { return m_ibo; }
};
