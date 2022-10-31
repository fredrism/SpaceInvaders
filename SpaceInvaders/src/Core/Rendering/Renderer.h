#pragma once
#include "GL_Debugger.h"
#include <vector>
#include "Material.h"
#include "Geometry/BufferGeometry.h"
#include "glad/glad.h"
#include "../VMath/VecXX.h"
#include "../System.h"
#include "Geometry/RectGeometry.h"
#include "Geometry/TextGeometry.h"
#include "Geometry/LineGeometry.h"
#include "FrameBuffer.h"
#include "Effects/Bloom.h"

enum RenderQueue
{
	Opaque, Transparent
};

struct RenderCommand
{
	std::shared_ptr<Texture2D> texture;
	Vec4f* tint;
	Matrix4f* matrix;
	float z;
};

class Renderer : public System
{
public:
	Renderer();
	~Renderer();

	virtual void BeginFrame() override;

	void Draw(BufferGeometry* geom, Material* material);
	void Draw(BufferGeometry* geom, Material* material, Matrix4f* matrix);
	void DrawInstanced(BufferGeometry* geom, Material* material, Matrix4f* matrices, Vec4f* colors, int count);
	void DrawLines(BufferGeometry* geom, Material* material);
	void DrawText(TextGeometry* text, Matrix4f* matrix);
	void DrawThickline(LineGeometry* geometry, Material* material);
	void DrawSprite(std::shared_ptr<Texture2D> texture, Vec4f* tint, Matrix4f* matrix);

	void SetClearColor(Vec4f color) { glClearColor(color.r, color.g, color.b, color.a); };
	void SetViewportSize(Vec2i size);
	Vec2f ScreenToViewPoint(Vec2f point);

	void Blit();

	void LateUpdate();

	void Submit(RenderCommand cmd);
	void Clear() 
	{ 
		m_renderQueue.clear(); 
	}

private:
	void DrawRenderQueue();

	std::vector<RenderCommand> m_renderQueue;
	std::shared_ptr<Shader> m_spriteShader;
	std::shared_ptr<Shader> m_blitShader;
	std::unique_ptr<Bloom> m_bloom;
	RectGeometry m_quad;
	Vec2i m_screenSize;

	FrameBuffer m_fbo;

	unsigned int spriteMatrixLocation;
	unsigned int spriteTintLocation;
	unsigned int spriteTextureLocation;
	unsigned int blitTextureLocation;
};

