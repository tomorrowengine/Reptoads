#pragma once
#include "memory/Containers.h"
#include "memory/smart_ptr.h"
#include "Assertion.h"

namespace core
{
	template<typename T>
	class TreeNode;

	
	
	template<typename T, bool Const>
	class TTreePerLayerIterator
	{
	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = TreeNode<T>;
		using difference_type = std::ptrdiff_t;
		using pointer = TreeNode<T>* ;
		using reference = TreeNode<T>& ;

		TTreePerLayerIterator() = default;
		explicit TTreePerLayerIterator(pointer root)
		{
			openSet_.push(root);
		}

	private:
		ptl::queue<pointer> openSet_;

	public:
		void swap(TTreePerLayerIterator& other) noexcept
		{
			std::swap(openSet_, other.openSet_);
		}

		TTreePerLayerIterator& operator++ () 
		{
			ASSERT_MSG(!openSet_.empty(), "Invalid iterator increment!");

			pointer widget = openSet_.front();
			openSet_.pop();

			for (auto& child : widget->GetChildren())
				openSet_.push(child.get());

			return *this;
		}

		TTreePerLayerIterator operator++ (int) 
		{
			ASSERT(!openSet_.empty() && "Out-of-bounds iterator increment!");
			TTreePerLayerIterator tmp(*this);
			++(*this);
			return tmp;
		}

		
		bool operator == (const TTreePerLayerIterator& rhs) const
		{
			return openSet_ == rhs.openSet_;
		}

		bool operator != (const TTreePerLayerIterator& rhs) const
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

		
		operator TTreePerLayerIterator<T, true>() const
		{
			return TTreePerLayerIterator(*this);
		}
	};

	template<typename T>
	using TreePerLayerIterator = TTreePerLayerIterator<T, false>;
	template<typename T>
	using ConstTreePerLayerIterator = TTreePerLayerIterator<T, true>;

	
	template<typename T>
	class TreeNode
	{
		TreeNode* parent_ = nullptr;
		ptl::vector<ptl::unique_ptr<TreeNode>> children_ {};

		T t_;
	public:
		TreeNode() = default;

		explicit TreeNode(const T& t) : t_(t)
		{
		}
		explicit TreeNode(T&& t) : t_(t)
		{
		}

		T& GetData() { return t_; }
		const T& GetData() const { return t_; }

		bool IsRoot() const { return parent_ == nullptr; }
		TreeNode* GetParent() { return parent_; }
		const TreeNode* GetParent() const{ return parent_; }

		void SetParent(TreeNode* newParent)
		{
			TreeNode* oldParent = parent_;
			auto it = std::find_if(oldParent->children_.begin(), oldParent->children_.end(), [this](const ptl::unique_ptr<TreeNode>& child) { return child.get() == this; });
			ASSERT_MSG(it != oldParent->children_.end(), 
				"For some reason, the old parent does not contain this node as a child.");
			TreeNode* thisNode = (*it).release();
			ASSERT_MSG(thisNode == this, "We are supposed to find ourselves, and then remove ourselves as child, but we have not found ourselves??");
			oldParent->children_.erase(it);

			parent_ = newParent;
			if(parent_ != nullptr)
				parent_->AddChild(ptl::unique_ptr<TreeNode>{thisNode});
		}

		const ptl::vector<ptl::unique_ptr<TreeNode>>& GetChildren() const { return children_; }
		TreeNode* AddChild(ptl::unique_ptr<TreeNode>&& child)
		{
			TreeNode* ptr = child.get();

			children_.emplace_back(std::move(child));

			return ptr;
		}

		TreePerLayerIterator<T> BeginLayerByLayer() { return TreePerLayerIterator<T>{this}; }
		ConstTreePerLayerIterator<T> BeginLayerByLayer() const{ return ConstTreePerLayerIterator<T>{this}; }
		TreePerLayerIterator<T> EndLayerByLayer() { return TreePerLayerIterator<T>{nullptr}; }
		ConstTreePerLayerIterator<T> EndLayerByLayer() const { return ConstTreePerLayerIterator<T>{nullptr}; }
	};

	template<typename T>
	using Tree = TreeNode<T>;
}
