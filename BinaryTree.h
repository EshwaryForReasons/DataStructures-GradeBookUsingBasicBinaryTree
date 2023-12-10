
#pragma once

#include <stack>
#include <queue>
#include <functional>

using namespace std;

template<typename T>
class Node
{
public:

	T* data = nullptr;
	Node<T>* left = nullptr;
	Node<T>* right = nullptr;
};

template<typename T>
class BinaryTree
{
public:

	void insert(T* new_element);
	const queue<T*> traverse();
	void clear();

	//Comparison function true if current > to_find
	Node<T>* search_node(std::function<bool(void* current)> continue_check_function, std::function<bool(void* current)> comparision_function);

protected:

	Node<T>* root;

	void insert(Node<T>* _root, Node<T>* new_node);
};

template<typename T>
Node<T>* BinaryTree<T>::search_node(std::function<bool(void* current)> continue_check_function, std::function<bool(void* current)> comparision_function)
{
	Node<T>* current = root;
	while (current && continue_check_function(current))
	{
		//Since the tree is sorted by name, we can just compare names
		if (comparision_function(current))
			current = current->left;
		else if (!comparision_function(current))
			current = current->right;
	}
	return current;
}

template<typename T>
void BinaryTree<T>::clear()
{
	//To clear, I just store all the nodes in a stack and delete them one by one
	stack<Node<T>*> to_delete;

	stack<Node<T>*> node_stack;
	Node<T>* current = root;

	while (!node_stack.empty() || current)
	{
		while (current)
		{
			node_stack.push(current);
			current = current->left;
		}

		current = node_stack.top();
		node_stack.pop();

		to_delete.push(current);

		current = current->right;
	}

	while (!to_delete.empty())
	{
		delete to_delete.top();
		to_delete.pop();
	}

	root = nullptr;
}

template<typename T>
void BinaryTree<T>::insert(T* new_element)
{
	Node<T>* new_node = new Node<T>;
	new_node->data = new_element;
	new_node->left = nullptr;
	new_node->right = nullptr;

	if (root == nullptr)
	{
		root = new_node;
	}
	else
	{
		insert(root, new_node);
	}
}

template<typename T>
void BinaryTree<T>::insert(Node<T>* _root, Node<T>* new_node)
{
	if (*new_node->data < *_root->data)
	{
		if (_root->left == nullptr)
		{
			_root->left = new_node;
		}
		else
		{
			insert(_root->left, new_node);
		}
	}
	else if (*new_node->data > *_root->data)
	{
		if (_root->right == nullptr)
		{
			_root->right = new_node;
		}
		else
		{
			insert(_root->right, new_node);
		}
	}
}

template<typename T>
const queue<T*> BinaryTree<T>::traverse()
{
	queue<T*> traverse_queue;

	stack<Node<T>*> node_stack;
	Node<T>* current = root;

	while (!node_stack.empty() || current)
	{
		while (current)
		{
			node_stack.push(current);
			current = current->left;
		}

		current = node_stack.top();
		node_stack.pop();

		traverse_queue.push(current->data);

		current = current->right;
	}

	return traverse_queue;
}
