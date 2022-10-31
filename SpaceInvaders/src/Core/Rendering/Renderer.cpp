#include "pch.h"
#include "Renderer.h"
#include "../Util/Profiler.h"
#include "../Resources/ShaderLoader.h"

Renderer::Renderer() : m_quad(Vec2f(-1, -1), Vec2f(1, 1)), m_fbo(GL_RGBA, GL_RGBA16F, 640, 480)
{
	m_spriteShader = ShaderLoader::Load("resources/shaders/unlit_texture.glsl");
	m_blitShader = ShaderLoader::Load("resources/shaders/blit.glsl");
	m_bloom = std::make_unique<Bloom>(640, 480);

	GLDEBUG(spriteMatrixLocation = glGetUniformLocation(m_spriteShader->m_id, "u_transform"));
	GLDEBUG(spriteTextureLocation = glGetUniformLocation(m_spriteShader->m_id, "u_texture"));
	GLDEBUG(spriteTintLocation = glGetUniformLocation(m_spriteShader->m_id, "u_tint"));
	GLDEBUG(blitTextureLocation = glGetUniformLocation(m_blitShader->m_id, "u_texture"));

	GLDEBUG(glViewport(0, 0, 640, 480));
	m_screenSize = Vec2i(640, 480);
}

Renderer::~Renderer()
{
}

void Renderer::BeginFrame()
{
	m_fbo.Bind();
	GLDEBUG(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(BufferGeometry* geom, Material* material)
{
	geom->Bind();
	material->Use();

	GLDEBUG(glDrawElements(GL_TRIANGLES, geom->m_ib.Count(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw(BufferGeometry* geom, Material* material, Matrix4f* matrix)
{
	geom->Bind();
	material->Use();
	//SI_SCOPE_PROFILE("Renderer->Draw");

	if (material->BlendMode())
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		glDisable(GL_BLEND);
	}

	GLDEBUG(unsigned int transformLocation = glGetUniformLocation(material->GetShader()->m_id, "u_transform"));
	GLDEBUG(glUniformMatrix4fv(transformLocation, 1, GL_FALSE, (float*)matrix));

	GLDEBUG(glDrawElements(GL_TRIANGLES, geom->m_ib.Count(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawLines(BufferGeometry* geom, Material* material)
{
	geom->Bind();
	material->Use();

	GLDEBUG(glDrawElements(GL_LINES, geom->m_ib.Count(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawInstanced(BufferGeometry* geom, Material* material, Matrix4f* matrices, Vec4f* colors, int count)
{
	geom->Bind();
	material->Use();

	GLDEBUG(unsigned int colorLocation = glGetUniformLocation(material->GetShader()->m_id, "u_colors"));
	GLDEBUG(unsigned int transformLocation = glGetUniformLocation(material->GetShader()->m_id, "u_transforms"));

	GLDEBUG(glUniform4fv(colorLocation, count, (float*)colors));
	GLDEBUG(glUniformMatrix4fv(transformLocation, count, GL_FALSE, (float*)matrices));

	GLDEBUG(glDrawElementsInstanced(GL_TRIANGLES, geom->m_ib.Count(), GL_UNSIGNED_INT, nullptr, count));
}

void Renderer::DrawText(TextGeometry* text, Matrix4f* matrix)
{
	text->m_rect.geometry.Bind();
	text->m_textMaterial->Use();

	int size = text->m_text.size();
	if (size > 64) size = 64;

	GLDEBUG(unsigned int transformLocation = glGetUniformLocation(text->m_textMaterial->GetShader()->m_id, "u_transform"));
	GLDEBUG(unsigned int scaleLocation = glGetUniformLocation(text->m_textMaterial->GetShader()->m_id, "u_scale"));
	GLDEBUG(unsigned int offsetLocation = glGetUniformLocation(text->m_textMaterial->GetShader()->m_id, "u_offset"));

	GLDEBUG(glUniformMatrix4fv(transformLocation, 1, GL_FALSE, (float*)matrix));
	GLDEBUG(glUniform2fv(scaleLocation, 1, (float*)&text->m_scale));
	GLDEBUG(glUniform2fv(offsetLocation, size, (float*)&text->m_offsets[0]));

	GLDEBUG(glDrawElementsInstanced(GL_TRIANGLES, text->m_rect.geometry.m_ib.Count(), GL_UNSIGNED_INT, nullptr, size));
}

void Renderer::DrawThickline(LineGeometry* geometry, Material* material)
{
	geometry->geometry.Bind();
	material->Use();

	GLDEBUG(glDrawElements(GL_LINES, geometry->geometry.m_ib.Count(), GL_UNSIGNED_INT, nullptr));
}

Vec2f Renderer::ScreenToViewPoint(Vec2f point)
{
	return Vec2f(2 * (point.x/m_screenSize.x) - 1, 1 - 2 * (point.y /m_screenSize.y));
}

void Renderer::LateUpdate()
{
	DrawRenderQueue();

	m_fbo.Unbind();
	glDisable(GL_BLEND);

	m_quad.geometry.Bind();
	m_bloom->Apply(&m_fbo);
}

void Renderer::DrawSprite(std::shared_ptr<Texture2D> texture, Vec4f* tint, Matrix4f* matrix)
{
	m_quad.geometry.Bind();
	m_spriteShader->Bind();
	texture->Bind(0);
	//SI_SCOPE_PROFILE("Renderer->Draw");
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLDEBUG(glUniformMatrix4fv(spriteMatrixLocation, 1, GL_FALSE, (float*)matrix));
	GLDEBUG(glUniform1i(spriteTextureLocation, 0));
	GLDEBUG(glUniform4fv(spriteTintLocation, 1, (float*)tint));

	GLDEBUG(glDrawElements(GL_TRIANGLES, m_quad.geometry.m_ib.Count(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Submit(RenderCommand cmd)
{
	m_renderQueue.push_back(cmd);
}

void Renderer::Blit()
{
	GLDEBUG(glDrawElements(GL_TRIANGLES, m_quad.geometry.m_ib.Count(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawRenderQueue()
{
	std::sort(m_renderQueue.begin(), m_renderQueue.end(), [](RenderCommand a, RenderCommand b) {
		return a.z > b.z;
	});

	for (auto& cmd : m_renderQueue)
	{
		DrawSprite(cmd.texture, cmd.tint, cmd.matrix);
	}
}

void Renderer::SetViewportSize(Vec2i newSize)
{
	m_screenSize = newSize;
	m_fbo.Resize(newSize.x, newSize.y);
	m_bloom->Resize(newSize.x, newSize.y);
	glViewport(0, 0, newSize.x, newSize.y);
}
