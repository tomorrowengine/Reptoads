#pragma once
#include "memory/Containers.h"
#include "gameplay/Transform.h"
#include "rendering/Renderable.h"
#include "memory/smart_ptr.h"
#include "core/Assertion.h"
#include <algorithm>

namespace scene
{
	class SceneNode;

	
	
	template<bool Const>
	class TSceneNodePerLayerIterator
	{
	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = SceneNode;
		using difference_type = std::ptrdiff_t;
		using pointer = SceneNode *;
		using reference = SceneNode &;

		TSceneNodePerLayerIterator() = default;
		explicit TSceneNodePerLayerIterator(pointer root)
		{
			ASSERT(root != nullptr);

			openSet_.push(root);
		}

	private:
		ptl::queue<pointer> openSet_;

	public:
		void swap(TSceneNodePerLayerIterator& other) noexcept
		{
			std::swap(openSet_, other.openSet_);
		}

		TSceneNodePerLayerIterator& operator++() 
		;

		TSceneNodePerLayerIterator operator++ (int) 
		{
			ASSERT(!openSet_.empty() && "Out-of-bounds iterator increment!");
			TSceneNodePerLayerIterator tmp(*this);
			++(*this);
			return tmp;
		}

		
		bool operator == (const TSceneNodePerLayerIterator& rhs) const
		{
			return openSet_ == rhs.openSet_;
		}

		bool operator != (const TSceneNodePerLayerIterator& rhs) const
		{
			return openSet_ != rhs.openSet_;
		}

		reference operator*() const
		{
			ASSERT(!openSet_.empty() && "Invalid iterator dereference!");
			return *openSet_.front();
		}

		pointer operator-> () const
		{
			ASSERT(!openSet_.empty() && "Invalid iterator dereference!");
			return openSet_.front();
		}

		
		operator TSceneNodePerLayerIterator< true>() const
		{
			return TSceneNodePerLayerIterator(*this);
		}
	};


	
	template<bool Const>
	class TSceneNodeIterator
	{
	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = SceneNode;
		using difference_type = std::ptrdiff_t;
		using pointer = SceneNode * ;
		using reference = SceneNode & ;

		TSceneNodeIterator() = default;
		explicit TSceneNodeIterator(pointer root)
		{
			ASSERT(root != nullptr);

			openSet_.push(root);
		}

	private:
		ptl::stack<pointer, ptl::vector<pointer>> openSet_;

	public:
		void swap(TSceneNodeIterator& other) noexcept
		{
			std::swap(openSet_, other.openSet_);
		}

		TSceneNodeIterator& operator++() 
		;

		TSceneNodeIterator operator++ (int) 
		{
			ASSERT(!openSet_.empty() && "Out-of-bounds iterator increment!");
			TSceneNodeIterator tmp(*this);
			++(*this);
			return tmp;
		}

		
		bool operator == (const TSceneNodeIterator& rhs) const
		{
			return openSet_ == rhs.openSet_;
		}

		bool operator != (const TSceneNodeIterator& rhs) const
		{
			return openSet_ != rhs.openSet_;
		}

		reference operator*() const
		{
			ASSERT(!openSet_.empty() && "Invalid iterator dereference!");
			return *openSet_.top();
		}

		pointer operator-> () const
		{
			ASSERT(!openSet_.empty() && "Invalid iterator dereference!");
			return openSet_.top();
		}

		
		operator TSceneNodeIterator< true>() const
		{
			return TSceneNodeIterator(*this);
		}
	};



	using SceneNodePerLayerIterator = TSceneNodePerLayerIterator<false>;
	using ConstSceneNodePerLayerIterator = TSceneNodePerLayerIterator<true>;

	using SceneNodeIterator = TSceneNodeIterator<false>;
	using ConstSceneNodeIterator = TSceneNodeIterator<true>;

    
	class SceneNode
	{
		SceneNode* parent_ = nullptr;
		ptl::vector<ptl::unique_ptr<SceneNode>> children_{};

	public:
		ptl::string name{};
		core::Transform transform;
		ptl::vector<gfx::Renderable> renderables{};

		SceneNode() = default;

	    SceneNode(const SceneNode& other); 
        SceneNode(SceneNode&& other) = default;
	    SceneNode& operator=(const SceneNode& other);
        SceneNode& operator=(SceneNode&& other) = default;

		SceneNode* AddSceneNode(core::Transform&& transform_, const ptl::vector<gfx::Renderable>& renderables_ = {})
		{
			SceneNode* child = new SceneNode();
			child->transform = transform_;
			child->transform.parent = &this->transform;
			child->renderables = renderables_;
			child->parent_ = this;
			children_.push_back(ptl::unique_ptr<SceneNode>(child));

			return child;
		}
		SceneNode* AddSceneNode(const core::Transform& transform_, const ptl::vector<gfx::Renderable>& renderables_ = {})
		{
			SceneNode* child = new SceneNode();
			child->transform = transform_;
			child->transform.parent = &this->transform;
			child->renderables = renderables_;
			child->parent_ = this;
			children_.push_back(ptl::unique_ptr<SceneNode>(child));

			return child;
		}

		
		void RemoveSceneNode(SceneNode* nodeToRemove)
		{
			ASSERT_MSG(!nodeToRemove->IsRoot(), "ERROR: Trying to remove root node from SceneGraph!");

			auto it = std::find_if(children_.begin(), children_.end(), [nodeToRemove](const ptl::unique_ptr<SceneNode>& node) {
				return node.get() == nodeToRemove;
			});

			ASSERT_MSG(it != children_.end(),
				"For some reason, the nodeToRemove has a parent which does not contain this nodeToRemove as a child.");

			children_.erase(it);
		}

		SceneNode* GetParent() { return parent_; }
		const SceneNode* GetParent() const { return parent_; }

		const ptl::vector<ptl::unique_ptr<SceneNode>>& GetChildren() const { return children_; }

		void SetParent(SceneNode* newParent)
		{
            SceneNode* thisNode{nullptr};
            if(parent_ != nullptr){
			    SceneNode* oldParent = parent_;
			    auto it = std::find_if(oldParent->children_.begin(), oldParent->children_.end(), [this](const ptl::unique_ptr<SceneNode>& child) { return child.get() == this; });
			    ASSERT_MSG(it != oldParent->children_.end(),
				    "For some reason, the old parent does not contain this node as a child.");

                thisNode = (*it).release();
			    ASSERT_MSG(thisNode == this, "We are supposed to find ourselves, and then remove ourselves as child, but we have not found ourselves??");
			    oldParent->children_.erase(it);
            }else
            {
                thisNode = this;
            }

			parent_ = newParent;
			if (parent_ != nullptr)
				parent_->AddSceneNode(ptl::unique_ptr<SceneNode>{thisNode});
		}

		bool IsRoot() const
		{
			return parent_ == nullptr;
		}

		SceneNodePerLayerIterator BeginLayerByLayer() { return SceneNodePerLayerIterator{ this }; }
		
		SceneNodePerLayerIterator EndLayerByLayer() { return SceneNodePerLayerIterator{ }; }
		

		
		SceneNodeIterator begin() { return SceneNodeIterator{ this }; }
		
		SceneNodeIterator end() { return SceneNodeIterator{ }; }

		
		SceneNodeIterator Begin() { return begin();}
		
		SceneNodeIterator End() { return end(); }

        void Clear()
		{
            parent_ = nullptr;
            children_.clear();
		}

	    
        SceneNode* FindNodeByName(ptl::string nodeName)
		{
            if (this != nullptr && this->name == nodeName) return this;
            for(auto& node : *this)
            {
                if (node.name == nodeName)
                    return &node;
            }
            
            
            
            
            
            
            return nullptr;
		}
        

	
		SceneNode* AddSceneNode(ptl::unique_ptr<SceneNode>&& child)
		{
			auto* ptr = child.get();

			child->parent_ = this;
			child->transform.parent = &this->transform;
			children_.push_back(std::move(child));

			return ptr;
		}
	};

    inline SceneNode::SceneNode(const SceneNode& cpy) : transform(cpy.transform), name(cpy.name), renderables(cpy.renderables), parent_(cpy.parent_) 
    {
        children_.reserve(cpy.children_.size());
        for (auto& ptrChild : cpy.children_) 
        {
            children_.emplace_back(new scene::SceneNode{ *ptrChild });
        }
    }
    inline SceneNode& SceneNode::operator=(const SceneNode& cpy) 
    {
        transform = cpy.transform;
        name = cpy.name;
        renderables = cpy.renderables;
        parent_ = cpy.parent_;

        children_.reserve(cpy.children_.size());
        for (auto& ptrChild : cpy.children_) 
        {
            children_.emplace_back(new scene::SceneNode{ *ptrChild });
        }

        return *this;
    }



    
	template <bool Const>
	TSceneNodeIterator<Const>& TSceneNodeIterator<Const>::operator++()
	{
		ASSERT_MSG(!openSet_.empty(), "Invalid iterator increment!");

		pointer widget = openSet_.top();
		openSet_.pop();

		for (auto& child : widget->GetChildren())
			openSet_.push(child.get());

		return *this;
	}

	template <bool Const>
	TSceneNodePerLayerIterator<Const>& TSceneNodePerLayerIterator<Const>::operator++()
	{
		ASSERT_MSG(!openSet_.empty(), "Invalid iterator increment!");

		pointer widget = openSet_.front();
		openSet_.pop();

		for (auto& child : widget->GetChildren())
			openSet_.push(child.get());

		return *this;
	}
}
