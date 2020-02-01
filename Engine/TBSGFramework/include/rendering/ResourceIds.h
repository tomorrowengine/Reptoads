#pragma once
#include <cstdint>

namespace gfx
{
	
	struct Identifier
	{
		explicit Identifier(uint32_t id) : _id(id){}
		Identifier(const Identifier& other) = default;
		Identifier& operator=(const Identifier& other) = default;
		Identifier(Identifier&& other) = default;
		Identifier& operator=(Identifier&& other) = default;
		uint32_t _id;
		
		
		bool IsNotNull() const { return _id != 0; }
		friend bool operator==(const Identifier& lhs, const Identifier& rhs) { return lhs._id == rhs._id; }
		friend bool operator!=(const Identifier& lhs, const Identifier& rhs) { return lhs._id != rhs._id; }
	};

	struct TextureId : public Identifier
	{
		explicit TextureId(uint32_t id = 0) : Identifier(id){}
		TextureId(const TextureId& other) = default;
		TextureId& operator=(const TextureId& other) = default;
		TextureId(TextureId&& other) = default;
		TextureId& operator=(TextureId&& other) = default;
		friend bool operator==(const TextureId& lhs, const TextureId& rhs) { return lhs._id == rhs._id; }
		friend bool operator!=(const TextureId& lhs, const TextureId& rhs) { return !(lhs == rhs); }
	};
	struct ShaderId : public Identifier
	{
		explicit ShaderId(uint32_t id = 0) : Identifier(id) {}
		ShaderId(const ShaderId& other) = default;
		ShaderId& operator=(const ShaderId& other) = default;
		ShaderId(ShaderId&& other) = default;
		ShaderId& operator=(ShaderId&& other) = default;
		friend bool operator==(const ShaderId& lhs, const ShaderId& rhs) { return lhs._id == rhs._id; }
		friend bool operator!=(const ShaderId& lhs, const ShaderId& rhs) { return !(lhs == rhs); }
	};
	struct MeshId : public Identifier
	{
		MeshId(const MeshId& other) = default;
		MeshId& operator=(const MeshId& other) = default;
		MeshId(MeshId&& other) = default;
		MeshId& operator=(MeshId&& other) = default;
		explicit MeshId(uint32_t id = 0) : Identifier(id) {}
		friend bool operator==(const MeshId& lhs, const MeshId& rhs) { return lhs._id == rhs._id; }
		friend bool operator!=(const MeshId& lhs, const MeshId& rhs) { return !(lhs == rhs); }
	};
	struct MaterialId : public Identifier
	{
		MaterialId(const MaterialId& other) = default;
		MaterialId& operator=(const MaterialId& other) = default;
		MaterialId(MaterialId&& other) = default;
		MaterialId& operator=(MaterialId&& other) = default;
		explicit MaterialId(uint32_t id = 0) : Identifier(id) {}
		friend bool operator==(const MaterialId& lhs, const MaterialId& rhs) { return lhs._id == rhs._id; }
		friend bool operator!=(const MaterialId& lhs, const MaterialId& rhs) { return !(lhs == rhs); }
	};

	struct RenderableId : public Identifier
	{
		RenderableId(const RenderableId& other) = default;
		RenderableId& operator=(const RenderableId& other) = default;
		RenderableId(RenderableId&& other) = default;
		RenderableId& operator=(RenderableId&& other) = default;
		explicit RenderableId(uint32_t id = 0) : Identifier(id) {}
		friend bool operator==(const RenderableId& lhs, const RenderableId& rhs) { return lhs._id == rhs._id; }
		friend bool operator!=(const RenderableId& lhs, const RenderableId& rhs) { return !(lhs == rhs); }
	};

	struct RenderPassId : public Identifier
	{
		explicit RenderPassId(uint32_t id = 0) : Identifier(id) {}
		RenderPassId(const RenderPassId& other) = default;
		RenderPassId& operator=(const RenderPassId& other) = default;
		RenderPassId(RenderPassId&& other) = default;
		RenderPassId& operator=(RenderPassId&& other) = default;
		friend bool operator==(const RenderPassId& lhs, const RenderPassId& rhs) { return lhs._id == rhs._id; }
		friend bool operator!=(const RenderPassId& lhs, const RenderPassId& rhs) { return !(lhs == rhs); }
	};

	struct ParticleEmitterId : public Identifier
	{
		explicit ParticleEmitterId(uint32_t id) : Identifier(id) {}
		ParticleEmitterId(const ParticleEmitterId& other) = default;
		ParticleEmitterId& operator=(const ParticleEmitterId& other) = default;
		ParticleEmitterId(ParticleEmitterId&& other) = default;
		ParticleEmitterId& operator=(ParticleEmitterId&& other) = default;
		friend bool operator==(const ParticleEmitterId& lhs, const ParticleEmitterId& rhs) { return lhs._id == rhs._id; }
		friend bool operator!=(const ParticleEmitterId& lhs, const ParticleEmitterId& rhs) { return !(lhs == rhs); }
	};
}

namespace std
{
	template<>
	struct hash<gfx::TextureId>
	{
		std::size_t operator()(const gfx::TextureId& id) const noexcept
		{
			return std::hash<uint32_t>{}(id._id);
		}
	};
	template<>
	struct hash<gfx::ShaderId>
	{
		std::size_t operator()(const gfx::ShaderId& id) const noexcept
		{
			return std::hash<uint32_t>{}(id._id);
		}
	};
	template<>
	struct hash<gfx::MeshId>
	{
		std::size_t operator()(const gfx::MeshId& id) const noexcept
		{
			return std::hash<uint32_t>{}(id._id);
		}
	};
	template<>
	struct hash<gfx::MaterialId>
	{
		std::size_t operator()(const gfx::MaterialId& id) const noexcept
		{
			return std::hash<uint32_t>{}(id._id);
		}
	};
	template<>
	struct hash<gfx::RenderableId>
	{
		std::size_t operator()(const gfx::RenderableId& id) const noexcept
		{
			return std::hash<uint32_t>{}(id._id);
		}
	};
	template<>
	struct hash<gfx::RenderPassId>
	{
		std::size_t operator()(const gfx::RenderPassId& id) const noexcept
		{
			return std::hash<uint32_t>{}(id._id);
		}
	};
	template<>
	struct hash<gfx::ParticleEmitterId>
	{
		std::size_t operator()(const gfx::ParticleEmitterId& id) const noexcept
		{
			return std::hash<uint32_t>{}(id._id);
		}
	};
}
