#pragma once
#include <concepts>
#include <string>
#include <functional>
#include <stack>
#include <queue>
#include <math.h>
#include <list>

template<typename T>
concept Hashable = requires(T a) {
	{ std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

template<typename T>
concept Comparable = requires(T a, T b) {
	{ a <=> b } -> std::convertible_to<std::strong_ordering>;
	{ a == b } -> std::convertible_to<bool>;
};

template<typename T>
concept CopyConstructible = std::is_copy_constructible<T>::value;

size_t lastOperationPassedNodes = 0; //for insert, at, erase

template <Comparable K, CopyConstructible V>
class BinaryTree
{
public:
	class const_iterator;
	class iterator;
	class reverse_iterator;
	class const_reverse_iterator;
private:

	struct Node {
		K key;
		V value;
		Node* left = nullptr;
		Node* right = nullptr;
	};

	Node* root = nullptr;
	size_t size_ = 0;

	void forEachInternal(std::function<void(K&, V&)>) const;
	void forEachInternal(std::function<void(Node*)>);
	void forEachHorizontalInternal(std::function<void(K&, V&, size_t depth, size_t ordinalNumber)>) const;

	Node* findRecursive(const K& key, Node* node, std::stack<Node*>& wayFromRoot) const;
	Node* eraseRecursive(Node* currentNode, const K& key, bool& success);
	Node* findMinElement(Node*);
	bool insertRecursive(const K& key, const V& value, Node* node);
	Node* findAndCreateIfNotExists(const K& key, Node* node);
	
	
public:
	void print();
	/*==========================================
	          RULE OF FIVE + DESTRUCTOR
	==========================================*/

	BinaryTree() {};
	BinaryTree(const BinaryTree& other);
	BinaryTree(BinaryTree&& other);
	BinaryTree& operator=(const BinaryTree& other);
	BinaryTree& operator=(BinaryTree&& other);
	~BinaryTree();

	/*==========================================
	                INFORMATIONAL
	==========================================*/

	// Returns true or false if tree contains provided key
	bool contains(const K& key) const;

	// Returns current nodes number
	size_t size() const;

	// Shows if tree is empty
	bool empty() const;

	// Returns the list of tree keys in order: {root, less elements, greater elements}
	std::list<K> keys() const;

	/*==========================================
					  ACCESS
	==========================================*/

	// Returns the iterator pointing at the leaf contains the provided key
	iterator find(const K& key);
	// Returns the iterator pointing at the leaf contains the provided key
	const_iterator find(const K& key) const;

	// Returns the iterator pointing at the element behind the last one
	iterator end();
	// Returns the const iterator pointing at the element behind the last one
	const_iterator cend() const;

	// Returns the iterator pointing at the first element of sorted leafs sequence
	iterator begin();
	// Returns the const iterator pointing at the element behind the last one
	const_iterator cbegin() const;

	// Returns the reverse iterator pointing at the element behind the last one
	// (before begin element in forward iterator notation)
	reverse_iterator rend();
	// Returns the const reverse iterator pointing at the element behind the last one
	// (before begin element in forward iterator notation)
	const_reverse_iterator crend() const;

	// Returns the reverse iterator pointing at the rbegin element
	reverse_iterator rbegin();
	// Returns the const reverse iterator pointing at the rbegin element
	const_reverse_iterator crbegin() const;

	// Access and modyfing operator. Creates if necessary and returns the leaf with the provided key
	V& operator[](const K&);

	// Access by key and modyfing operator. Throws out_of_range exception if tree doesn't contain the provided key
	V& at(const K&);
	// Access by key operator. Throws out_of_range exception if tree doesn't contain the provided key
	const V& at(const K&) const;

	/*==========================================
					  MODYFING
	==========================================*/

	// Inserts the new key:value pair in the tree
	bool insert(const K&, const V&);
	bool insert(std::pair<const K&, const V&> pair);
	
	// Removes the leaf with the corresponding key
	bool erase(const K&);
	
	// Clears the tree
	void clear();


	/*==========================================
				PATH THROUGH METHODS
	==========================================*/

	// In order iterative pass algorithm. Transfers every tree value into lambda argument
	void forEach(std::function<void(const K&, V&)>);
	void forEach(std::function<void(const K&, const V&)>) const;

	// Horizontal iterative path through algorithm. Can be used to represent tree as a table
	// Third argument is depth (>=0): displays how deep you should go to reach the leaf (row)
	// Fourth argument is ordinalNumber (>=0): displays the position in leafs horizontal sequence (coloumn)
	//  *(sequence gaps are possible if tree isn't completed)
	void forEachHorizontal(std::function<void(const K&, const V&)>) const;
	void forEachHorizontal(std::function<void(const K&, const V&, size_t depth)>) const;
	void forEachHorizontal(std::function<void(const K&, const V&, size_t depth, size_t ordinalNumber)>) const;


	/*==========================================
					ITERATORS
	==========================================*/

	// Abstract iterator interface. Highest level superclass
	class iterator_base {
	protected:
		friend class BinaryTree;
		BinaryTree<K, V>::Node* ptr = nullptr;
		std::stack<Node*> nodes;
		iterator_base(Node*);
		iterator_base() {};
		void copy(const iterator_base& other);
		std::pair<const K&, V&> get() const;
		// to override:
		virtual void goForward() = 0;
		virtual void goBackward() = 0;
	public:
		friend bool operator==(const iterator_base& one, const iterator_base& two) {
			return one.ptr == two.ptr;
		};
	};

	// Forward iterator superclass.
	// Provides internal heirs functional and comparing operations
	class forward_iterator_base : public iterator_base {
	protected:
		forward_iterator_base(Node* node) : iterator_base(node) {};
		void goForward() override;
		void goBackward() override;
	public:
		forward_iterator_base() {};
		friend std::strong_ordering operator<=>(const BinaryTree<K, V>::forward_iterator_base& one, const BinaryTree<K, V>::forward_iterator_base& two) {
			if (one.ptr == nullptr) {
				if (two.ptr == nullptr)
					return std::strong_ordering::equal;
				else
					return std::strong_ordering::greater;
			}
			if (two.ptr == nullptr) return std::strong_ordering::less;
			return one.ptr->key <=> two.ptr->key;
		};
	};
	
	// Reverse iterator superclass
	// Provides internal heirs functional and comparing operations
	class reverse_iterator_base : public iterator_base {
	protected:
		reverse_iterator_base(Node* node) : iterator_base(node) {};
		void goForward() override;
		void goBackward() override;
	public:
		reverse_iterator_base() {};
		friend std::strong_ordering operator<=>(const BinaryTree<K, V>::reverse_iterator_base& one, const BinaryTree<K, V>::reverse_iterator_base& two) {
			if (one.ptr == nullptr) {
				if (two.ptr == nullptr)
					return std::strong_ordering::equal;
				else
					return std::strong_ordering::greater;
			}
			if (two.ptr == nullptr) return std::strong_ordering::less;
			std::strong_ordering result = one.ptr->key <=> two.ptr->key;
			if (result == std::strong_ordering::equal) return result;
			if (result == std::strong_ordering::less) return std::strong_ordering::greater;
			return std::strong_ordering::less;
		};
	};

	// Constant forward iterator. Doesn't allow to change the value while passing through
	class const_iterator : public forward_iterator_base {
	private:
		friend class BinaryTree;
		const_iterator(Node* node) : forward_iterator_base(node) {};
	public:
		const_iterator() = default;
		const_iterator(const iterator& other) { this->copy((iterator_base&)other); };
		const_iterator(const const_iterator& other) = default;
		std::pair<const K&, const V&> operator*() const { return this->get(); };
		const_iterator& operator++() { this->goForward(); return *this; };
		const_iterator operator++(int) { const_iterator newVal = *this; this->goForward(); return newVal; };
		const_iterator& operator--() { this->goBackward(); return *this; };
		const_iterator operator--(int) { const_iterator newVal = *this; this->goBackward(); return newVal; };
	};

	// Forward iterator
	class iterator : public forward_iterator_base {
	private:
		friend class BinaryTree;
		iterator(Node* node) : forward_iterator_base(node) {};
	public:
		iterator() = default;
		iterator(const iterator& other) = default;
		std::pair<const K&, V&> operator*() const { return this->get(); };
		iterator& operator++() { this->goForward(); return *this; };
		iterator operator++(int) { iterator newVal = *this; this->goForward(); return newVal; };
		iterator& operator--() { this->goBackward(); return *this; };
		iterator operator--(int) { iterator newVal = *this; this->goBackward(); return newVal; };
	};

	// Constant reverse iterator. Provides backward going through the container. Doesn't allow to change the value while passing through.
	class const_reverse_iterator : public reverse_iterator_base {
	private:
		friend class BinaryTree;
		const_reverse_iterator(Node* node) : reverse_iterator_base(node) {};
	public:
		const_reverse_iterator() = default;
		const_reverse_iterator(const const_reverse_iterator& other) = default;
		const_reverse_iterator(const reverse_iterator& other) { this->copy((iterator_base&)other); };
		std::pair<const K&, const V&> operator*() const { return this->get(); };
		const_reverse_iterator& operator++() { this->goForward(); return *this; };
		const_reverse_iterator operator++(int) {const_reverse_iterator newVal = *this; this->goForward(); return newVal; };
		const_reverse_iterator& operator--() { this->goBackward(); return *this; };
		const_reverse_iterator operator--(int) { const_reverse_iterator newVal = *this; this->goBackward(); return newVal; };
	};

	// Reverse iterator. Provides backward going through the container.
	class reverse_iterator : public reverse_iterator_base {
	private:
		friend class BinaryTree;
		reverse_iterator(Node* node) : reverse_iterator_base(node) {};
	public:
		reverse_iterator() = default;
		reverse_iterator(const reverse_iterator& other) = default;
		std::pair<const K&, V&> operator*() const { return this->get(); };
		reverse_iterator& operator++() { this->goForward(); return *this; };
		reverse_iterator operator++(int) { reverse_iterator newVal = *this; this->goForward(); return newVal; };
		reverse_iterator& operator--() { this->goBackward(); return *this; };
		reverse_iterator operator--(int) { reverse_iterator newVal = *this; this->goBackward(); return newVal; };
	};

	/*==========================================
					SERVICE
	==========================================*/
	size_t getLastOpPassedNodesNum() { return lastOperationPassedNodes; }

};

/*==========================================================================================
===========================================================================================
===========================================================================================
===========================================================================================
===========================================================================================



								METHODS DEFINITION STARTS THERE



/*==========================================================================================

								  SUPPORTING RECURSIVE METHODS

===========================================================================================*/

template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::Node* BinaryTree<K, V>::findRecursive(const K& key, Node* node, std::stack<Node*>& wayFromRoot) const
{
	if (node == root) lastOperationPassedNodes = 0;
	lastOperationPassedNodes++;

	if (node == nullptr) 
		return nullptr;
	if (node->key == key)
		return node;
	if (key > node->key) {
		wayFromRoot.push(node);
		return findRecursive(key, node->right, wayFromRoot);
	}
	else {
		wayFromRoot.push(node);
		return findRecursive(key, node->left,wayFromRoot);
	}
}

template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::Node* BinaryTree<K, V>::findMinElement(Node* currentNode) {
	if (currentNode->left == nullptr) 
		return currentNode;
	lastOperationPassedNodes++;
	return findMinElement(currentNode->left);
}

template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::Node* BinaryTree<K, V>::eraseRecursive(Node* currentNode, const K& key, bool& success)
{		
		
	if (currentNode == root) lastOperationPassedNodes = 0;
	lastOperationPassedNodes++;

	// если нашли нужный элемент, начинаем процедуру удаления
	if (currentNode->key == key) {
		// обработка самого простого случая, вместо узла возвращается null 
		if (currentNode->left == nullptr && currentNode->right == nullptr) {
			return nullptr;
		}

		// обработка двух случаев, с только одним из поддеревьев 
		if (currentNode->left == nullptr) {
			return currentNode->right;
		}

		if (currentNode->right == nullptr) {
			return currentNode->left;
		}

		Node* minNodeInRightSubtree = findMinElement(currentNode->right);
		// заменили текущий элемент минимальным из правого поддерева
		currentNode->key = minNodeInRightSubtree->key;
		currentNode->value = minNodeInRightSubtree->value;

		// ищем в правом поддереве минимальный элемент, 
		// значение которого уже вставлено на место текущего
		currentNode->right = eraseRecursive(minNodeInRightSubtree,minNodeInRightSubtree->key,success);

		return currentNode;
	}

	// попадаем сюда, если элемент не был найден, 
	// просто проваливаемся в дерево глубже и глубже

	// производится рекурсивный вызов этой же функции,
	// при этом если элемент не будет найден,
	// то алгоритм просто будет возвращать существующую ссылку на поддерево,
	// которая присвоится в ту же позицию
	if (key < currentNode->key) {
		if (currentNode->left == nullptr) {
			success = false;
			return currentNode;
		}

		// проваливаемся в левое поддерево,
		// после рекурсивной отработки функции _deleteNode
		// будет возвращен текущий элемент,
		// который в предыдущем вызове будет присвоен
		currentNode->left = eraseRecursive(currentNode->left, key,success);

		// присваивание на рекурсивный уровень выше,
		// может быть как в левое поддерево,так и в правое,
		// на текущем уровне мы не знаем какое поддерево обрабатываем  
		return currentNode;
	}

	// аналогичная обработка для правого поддерева
	if (key > currentNode->key) {
		if (currentNode->right == nullptr) {
			success = false;
			return currentNode;
		}

		currentNode->right = eraseRecursive(currentNode->right, key,success);
		return currentNode;
	}
}



template<Comparable K, CopyConstructible V>
inline bool BinaryTree<K, V>::insertRecursive(const K& key, const V& value, Node* node)
{
		if (node == root) lastOperationPassedNodes = 0;
		lastOperationPassedNodes++;

	
		if (key == node->key) return false;
		if (key > node->key) {
			if (node->right == nullptr) {
				Node* newNode;
				newNode = new Node();
				newNode->left = nullptr;
				newNode->right = nullptr;
				newNode->key = key;
				newNode->value = value;
				node->right = newNode;
			}
			else {
				return insertRecursive(key, value, node->right);
			}
		}
		if (key < node->key) {
			if (node->left == nullptr) {
				Node* newNode;
				newNode = new Node();
				newNode->left = nullptr;
				newNode->right = nullptr;
				newNode->key = key;
				newNode->value = value;
				node->left = newNode;
			}
			else {
				return insertRecursive(key, value, node->left);
			}
		}

	return true;
}

template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::Node* BinaryTree<K, V>::findAndCreateIfNotExists(const K& key, Node* node)
{
	Node* newNode = nullptr;
	if (key == node->key) return node;
	if (key > node->key) {
		if (node->right == nullptr) {
			newNode = new Node();
			newNode->key = key;
			newNode->value = V();
			node->right = newNode;
		}
		else {
			return findAndCreateIfNotExists(key, node->right);
		}
	}
	if (key < node->key) {
		if (node->left == nullptr) {
			newNode = new Node();
			newNode->key = key;
			newNode->value = V();
			node->left = newNode;
		}
		else {
			return findAndCreateIfNotExists(key, node->left);
		}
	}

	size_++;
	return newNode;
}

/*==========================================================================================

								  RULE OF FIVE AND DESTRUCTOR

===========================================================================================*/

template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::BinaryTree(const BinaryTree<K, V>& other)
{
	other.forEachHorizontal([&](const K& key, const V& val) {
		this->insert(key, val);
		});
}
template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::BinaryTree(BinaryTree&& other)
{
	this->root = other.root;
	this->size_ = other.size_;
	other.root = nullptr;
}

template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>& BinaryTree<K, V>::operator=(const BinaryTree& other)
{
	if (this != &other) {
		this->clear();
		other.forEachHorizontal([&](const K& key, const V& val) {
			this->insert(key, val);
			});
	}
	return *this;
}

template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>& BinaryTree<K, V>::operator=(BinaryTree&& other)
{
	if (this != &other) {
		this->root = other.root;
		this->size_ = other.size_;
		other.root = nullptr;
	}
	return *this;
}

template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::~BinaryTree()
{
	clear();
}


/*==========================================================================================

								  INFORMATIONAL OPERATIONS

===========================================================================================*/

template<Comparable K, CopyConstructible V>
bool BinaryTree<K, V>::contains(const K& key) const {
	if (findRecursive(key,root))
		return true;
	else
		return false;
}

template<Comparable K, CopyConstructible V>
size_t BinaryTree<K, V>::size() const {
	return size_;
}

template<Comparable K, CopyConstructible V>
bool BinaryTree<K, V>::empty() const {
	return size_ == 0;
}

template<Comparable K, CopyConstructible V>
inline std::list<K> BinaryTree<K, V>::keys() const
{
	std::list<K> keys;
	if (root == nullptr) return keys;
	std::stack<Node*> nodes;
	nodes.push(root);
	while (!nodes.empty()) {
		Node* node = nodes.top();
		nodes.pop();
		keys.push_back(node->key);
		if (node->right) nodes.push(node->right);
		if (node->left) nodes.push(node->left);
	}
	return keys;
}

/*==========================================================================================

								  ACCESS OPERATIONS

===========================================================================================*/


template<Comparable K, CopyConstructible V>
BinaryTree<K, V>::iterator BinaryTree<K, V>::find(const K& key) {
	std::stack<Node*> wayFromRoot;
	Node* result = findRecursive(key, root,wayFromRoot);
	iterator resultinIterator(result);
	resultinIterator.nodes = wayFromRoot;
	return resultinIterator;
}


template<Comparable K, CopyConstructible V>
BinaryTree<K, V>::const_iterator BinaryTree<K, V>::find(const K& key) const {
	std::stack<Node*> wayFromRoot;
	Node* result = findRecursive(key, root, wayFromRoot);
	const_iterator resultinIterator(result);
	resultinIterator.nodes = wayFromRoot;
	return resultinIterator;
}

template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::iterator BinaryTree<K, V>::end()
{
	if (this->root == nullptr) return iterator();
	iterator a;
	Node* currentNode = this->root;
	while (currentNode) {
		a.nodes.push(currentNode);
		currentNode = currentNode->right;
	}
	a.ptr = currentNode;
	return a;
}

template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::const_iterator BinaryTree<K, V>::cend() const
{
	if (this->root == nullptr) return iterator();
	iterator a;
	Node* currentNode = this->root;
	while (currentNode) {
		a.nodes.push(currentNode);
		currentNode = currentNode->right;
	}
	a.ptr = currentNode;
	return a;
}

template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::iterator BinaryTree<K, V>::begin()
{
	if (this->root == nullptr) return iterator();
	iterator a;
	Node* currentNode = this->root;
	while (currentNode->left) {
		a.nodes.push(currentNode);
		currentNode = currentNode->left;
	}
	a.ptr = currentNode;
	return a;
}
template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::const_iterator BinaryTree<K, V>::cbegin() const
{
	const_iterator a;
	Node* currentNode = this->root;
	while (currentNode->left) {
		a.nodes.push(currentNode);
		currentNode = currentNode->left;
	}
	a.ptr = currentNode;
	return a;

}

template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::reverse_iterator BinaryTree<K, V>::rend()
{
	if (this->root == nullptr) return reverse_iterator();
	reverse_iterator a;
	Node* currentNode = this->root;
	while (currentNode) {
		a.nodes.push(currentNode);
		currentNode = currentNode->left;
	}
	a.ptr = currentNode;
	return a;
}

template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::const_reverse_iterator BinaryTree<K, V>::crend() const
{
	if (this->root == nullptr) return reverse_iterator();
	reverse_iterator a;
	Node* currentNode = this->root;
	while (currentNode) {
		a.nodes.push(currentNode);
		currentNode = currentNode->left;
	}
	a.ptr = currentNode;
	return a;
}

template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::reverse_iterator BinaryTree<K, V>::rbegin()
{
	if (this->root == nullptr) return reverse_iterator();
	reverse_iterator a;
	Node* currentNode = this->root;
	while (currentNode->right) {
		a.nodes.push(currentNode);
		currentNode = currentNode->right;
	}
	a.ptr = currentNode;
	return a;
}

template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::const_reverse_iterator BinaryTree<K, V>::crbegin() const
{
	if (this->root == nullptr) return reverse_iterator();
	reverse_iterator a;
	Node* currentNode = this->root;
	while (currentNode->right) {
		a.nodes.push(currentNode);
		currentNode = currentNode->right;
	}
	a.ptr = currentNode;
	return a;
}


template<Comparable K, CopyConstructible V>
inline V& BinaryTree<K, V>::operator[](const K& key)
{

	Node* currentNode = root;

	if (currentNode == nullptr) {
		Node* newNode = new Node();
		newNode->key = key;
		root = newNode;
		size_++;
		return root->value;
	}

	return findAndCreateIfNotExists(key, root)->value;
}

template<Comparable K, CopyConstructible V>
inline V& BinaryTree<K, V>::at(const K& key) {
	auto it = (find(key));
	if (it.ptr == nullptr) throw std::out_of_range("operation at: no such key in the tree");
	return (*it).second;
}

template<Comparable K, CopyConstructible V>
inline const V& BinaryTree<K, V>::at(const K& key) const {
	auto it = (find(key));
	if (it.ptr == nullptr) throw std::out_of_range("operation at: no such key in the tree");
	return (*it).second;
}

/*==========================================================================================

								  MODIFYING OPERATIONS

===========================================================================================*/

template<Comparable K, CopyConstructible V>
inline bool BinaryTree<K, V>::insert(const K& key, const V& value)
{
	Node* currentNode = root;

	if (currentNode == nullptr) {
		Node* newNode = new Node();
		newNode->key = key;
		newNode->value = value;
		root = newNode;
		++size_;
		return true;
	}
	bool result = insertRecursive(key, value, root);
	if (result) ++size_;
	return result;
}

template<Comparable K, CopyConstructible V>
inline bool BinaryTree<K, V>::insert(std::pair<const K&, const V&> pair)
{
	return insert(pair.first, pair.second);
}

template<Comparable K, CopyConstructible V>
inline bool BinaryTree<K, V>::erase(const K& key)
{
	bool success = true;
	root = eraseRecursive(root, key, success);
	if (root) --size_;
	return success;
}

template<Comparable K, CopyConstructible V>
inline void BinaryTree<K, V>::clear()
{
	std::stack<Node*> toDelete;
	forEachInternal([&](Node* node) {
		toDelete.push(node);
	});

	while (!toDelete.empty()) {
		delete toDelete.top();
		toDelete.pop();
	}
	root = nullptr;
	size_ = 0;
}

/*==========================================================================================

                                  PASS THROUGH OPERATIONS

===========================================================================================*/

template<Comparable K, CopyConstructible V>
inline void BinaryTree<K, V>::forEachInternal(std::function<void(K&, V&)> func) const {
	Node* root = this->root;
	std::stack<Node*> nodes;
	while (root != nullptr || !nodes.empty()) {
		if (!nodes.empty()) {
			root = nodes.top();
			nodes.pop();
			func(root->key, root->value);
			if (root->right != nullptr)
				root = root->right;
			else
				root = nullptr;
		}
		while (root != nullptr) {
			nodes.push(root);
			root = root->left;
		}
	}

}


template<Comparable K, CopyConstructible V>
inline void BinaryTree<K, V>::forEachInternal(std::function<void(typename BinaryTree<K, V>::Node*)> func) {
	Node* root = this->root;
	std::stack<Node*> nodes;
	while (root != nullptr || !nodes.empty()) {
		if (!nodes.empty()) {
			root = nodes.top();
			nodes.pop();
			func(root);
			if (root->right != nullptr)
				root = root->right;
			else
				root = nullptr;
		}
		while (root != nullptr) {
			nodes.push(root);
			root = root->left;
		}
	}

}


template<Comparable K, CopyConstructible V>
inline void BinaryTree<K, V>::forEachHorizontalInternal(std::function<void(K& key, V& val, size_t depth, size_t ordinalNumber)> func) const {
	if (this->root == nullptr) return;
	Node* root = this->root;
	size_t depth = 0;
	size_t ordinalNumber = 0;
	struct nodeToGo {
		Node* root;
		size_t depth;
		size_t ordinalNumber;
		nodeToGo(Node* root_, size_t depth_, size_t ordinalNumber_) : root(root_), depth(depth_), ordinalNumber(ordinalNumber_) {};
	};
	std::queue<nodeToGo> nodes;
	do {
		if (!nodes.empty()) nodes.pop();

		func(root->key, root->value, depth, ordinalNumber);

		if (root->left != nullptr) nodes.push(nodeToGo(root->left, depth + 1, ordinalNumber * 2));
		if (root->right != nullptr) nodes.push(nodeToGo(root->right, depth + 1, ordinalNumber * 2 + 1));
		if (!nodes.empty()) {
			root = nodes.front().root;
			depth = nodes.front().depth;
			ordinalNumber = nodes.front().ordinalNumber;
		}
	} while (!nodes.empty());
}
template<Comparable K, CopyConstructible V>
inline void BinaryTree<K, V>::forEach(std::function<void(const K&, V&)> func) {
	forEachInternal((std::function<void(K&, V&)>)func);
}

template<Comparable K, CopyConstructible V>
inline void BinaryTree<K, V>::forEach(std::function<void(const K&, const V&)> func) const {
	forEachInternal((std::function<void(K&, V&)>)func);
}

template<Comparable K, CopyConstructible V>
inline void BinaryTree<K, V>::forEachHorizontal(std::function<void(const K&, const V&)> func) const {

	auto function = ([&](K& key, V& val, size_t depth, size_t ordinalNumber) {
		func(key, val);
		});
	forEachHorizontalInternal(function);
}
template<Comparable K, CopyConstructible V>
inline void BinaryTree<K, V>::forEachHorizontal(std::function<void(const K&, const V&, size_t)> func) const {

	auto function = ([&](K& key, V& val, size_t depth, size_t ordinalNumber) {
		func(key, val, depth);
		});
	forEachHorizontalInternal(function);
}
template<Comparable K, CopyConstructible V>
inline void BinaryTree<K, V>::forEachHorizontal(std::function<void(const K&, const V&, size_t, size_t)> func) const {

	auto function = ([&](K& key, V& val, size_t depth, size_t ordinalNumber) {
		func(key, val, depth, ordinalNumber);
		});
	forEachHorizontalInternal(function);
}

/*==========================================================================================

								  ITERATORS REALISATION

===========================================================================================*/


template<Comparable K, CopyConstructible V>
BinaryTree<K, V>::iterator_base::iterator_base(BinaryTree::Node* node) {
	ptr = node;
}

template<Comparable K, CopyConstructible V>
inline void BinaryTree<K, V>::iterator_base::copy(const iterator_base& other)
{
	this->ptr = other.ptr;
}

template<Comparable K, CopyConstructible V>
inline std::pair<const K&, V&> BinaryTree<K, V>::iterator_base::get() const
{
	if (this->ptr == nullptr) throw new std::logic_error("Iterator operation *: can't get the value of the end/rend node");
	return std::pair<const K&, V&>(this->ptr->key, this->ptr->value);
}


template<Comparable K, CopyConstructible V>
inline void BinaryTree<K, V>::forward_iterator_base::goForward()
{
	if (this->ptr == nullptr) throw std::logic_error("Iterator forward operation: can't go through the end node");
	
	// Get the right node if it's possible
	if (this->ptr->right) {
		this->nodes.push(this->ptr);
		this->ptr = this->ptr->right;
		// Go down through the left branch
		while (this->ptr->left) {
			this->nodes.push(this->ptr);
			this->ptr = this->ptr->left;
		}
		return;

	}
	// If there is no right nodes : go to the first parent with the bigger key
	std::stack<Node*> nodesTemp = this->nodes;
	while (!nodesTemp.empty() && nodesTemp.top()->key < this->ptr->key) {
		nodesTemp.pop();
	}
	// If that parent exists set iterator to it
	if (!nodesTemp.empty()) {
		this->ptr = nodesTemp.top();
		nodesTemp.pop();
		this->nodes = nodesTemp;
	}
	// If that parent doesn't exists: we are in the end node
	else {
		this->nodes.push(this->ptr);
		this->ptr = nullptr;
	}

}

template<Comparable K, CopyConstructible V>
inline void BinaryTree<K, V>::forward_iterator_base::goBackward()
{
	if (this->ptr == nullptr) {
		if (!this->nodes.empty()) {
			this->ptr = this->nodes.top();
			this->nodes.pop();
			return;
		}
		else {
			throw std::logic_error("Iterator going back operation: can't go through the begin node");
		}
	}

	// Get the left node if it's possible
	if (this->ptr->left) {
		this->nodes.push(this->ptr);
		this->ptr = this->ptr->left;
		// Go down through the right branch
		while (this->ptr->right) {
			this->nodes.push(this->ptr);
			this->ptr = this->ptr->right;
		}
		return;

	}
	// If there is no right nodes : go to the first parent with the less key
	std::stack<Node*> nodesTemp = this->nodes;
	while (!nodesTemp.empty() && nodesTemp.top()->key > this->ptr->key) {
		nodesTemp.pop();
	}
	// If that parent exists set iterator to it
	if (!nodesTemp.empty()) {
		this->ptr = nodesTemp.top();
		nodesTemp.pop();
		this->nodes = nodesTemp;
	}
	// If that parent doesn't exists: we are in the begin node
	else {
		throw std::logic_error("Iterator going back operation: can't go through the begin node");
	}
}

template<Comparable K, CopyConstructible V>
inline void BinaryTree<K, V>::reverse_iterator_base::goForward()
{
	if (this->ptr == nullptr) throw std::logic_error("Reverse iterator forward operation: can't go through the rend node");

	// Get the left node if it's possible
	if (this->ptr->left) {
		this->nodes.push(this->ptr);
		this->ptr = this->ptr->left;
		// Go down through the right branch
		while (this->ptr->right) {
			this->nodes.push(this->ptr);
			this->ptr = this->ptr->right;
		}
		return;

	}
	// If there is no right nodes : go to the first parent with the less key
	std::stack<Node*> nodesTemp = this->nodes;
	while (!nodesTemp.empty() && nodesTemp.top()->key > this->ptr->key) {
		nodesTemp.pop();
	}
	// If that parent exists set iterator to it
	if (!nodesTemp.empty()) {
		this->ptr = nodesTemp.top();
		nodesTemp.pop();
		this->nodes = nodesTemp;
	}
	// If that parent doesn't exists: we are in the end node
	else {
		this->nodes.push(this->ptr);
		this->ptr = nullptr;
	}
}

template<Comparable K, CopyConstructible V>
inline void BinaryTree<K, V>::reverse_iterator_base::goBackward()
{
	if (this->ptr == nullptr) {
		if (!this->nodes.empty()) {
			this->ptr = this->nodes.top();
			this->nodes.pop();
			return;
		}
		else {
			throw std::logic_error("Reverse iterator going back operation: can't go through the rbegin node");
		}
	}

	// Get the right node if it's possible
	if (this->ptr->right) {
		this->nodes.push(this->ptr);
		this->ptr = this->ptr->right;
		// Go down through the left branch
		while (this->ptr->left) {
			this->nodes.push(this->ptr);
			this->ptr = this->ptr->left;
		}
		return;

	}
	// If there is no right nodes : go to the first parent with the bigger key
	std::stack<Node*> nodesTemp = this->nodes;
	while (!nodesTemp.empty() && nodesTemp.top()->key < this->ptr->key) {
		nodesTemp.pop();
	}
	// If that parent exists set iterator to it
	if (!nodesTemp.empty()) {
		this->ptr = nodesTemp.top();
		nodesTemp.pop();
		this->nodes = nodesTemp;
	}
	// If that parent doesn't exists: we are in the begin node
	else {
		throw std::logic_error("Reverse iterator going back operation: can't go through the rbegin node");
	}
}

template<Comparable K, CopyConstructible V>
inline void BinaryTree<K, V>::print()
{

	int MAXIMUM_LEVEL = 5;

	if (this->root == nullptr) return;
	Node* root = this->root;
	size_t depth = 0;
	size_t ordinalNumber = 0;

	struct triplet {
		Node* node;
		size_t depth;
		size_t ordinalNumber;
		triplet(Node* node, size_t depth, size_t ordinalNumber) {
			this->node = node; this->depth = depth; this->ordinalNumber = ordinalNumber;
		}
	};

	std::vector<std::vector<Node*>> vectors;
	for (int i = 0; i < pow(2, MAXIMUM_LEVEL); i++) {
		std::vector<Node*> innerVector;
		for (int j = 0; j < pow(2, MAXIMUM_LEVEL); j++) {
			innerVector.push_back(nullptr);
		}
		vectors.push_back(innerVector);
	}
	size_t maxDepth;
	std::queue<triplet> nodes;
	do {
		if (!nodes.empty()) nodes.pop();
		vectors[depth][ordinalNumber] = root;
		if (root->left != nullptr) nodes.push(triplet(root->left, depth + 1, ordinalNumber * 2));
		if (root->right != nullptr) nodes.push(triplet(root->right, depth + 1, ordinalNumber * 2 + 1));
		maxDepth = depth + 1;
		if (!nodes.empty()) {
			root = nodes.front().node;
			depth = nodes.front().depth;
			ordinalNumber = nodes.front().ordinalNumber;
		}
	} while (!nodes.empty());
	size_t maxKeySize = 3;
	size_t maxSize = 3;
	int maxValuesNumber = pow(2, maxDepth);
	int symbolsInString = maxKeySize * maxValuesNumber + maxValuesNumber; 
	for (int i = 0; i <= maxDepth && i <= MAXIMUM_LEVEL; i++) {
		int thisStringValuesNumber = pow(2, i);
		for (int j = 0; j < thisStringValuesNumber; j++) {
			int probelsAfterEveryKey = (symbolsInString - (thisStringValuesNumber * maxKeySize)) / thisStringValuesNumber / 2;
			for (int s = 0; s < probelsAfterEveryKey; s++) std::cout << " ";
			if (vectors[i][j] == nullptr) {
				for (int s = 0; s < maxKeySize; s++) std::cout << " ";
			}
			else {
				std::cout << vectors[i][j]->key;
			}
			for (int s = 0; s < probelsAfterEveryKey; s++) std::cout << " ";
		}
		maxSize *= 2;
		std::cout << "\n";
	}
}

/*

template<Comparable K, CopyConstructible V>
inline bool BinaryTree<K, V>::erase(const K& key)
{
	Node* previousNode = nullptr;
	Node* currentNode = root;
	while (true) {
		//Found key in currentNode
		if (key == currentNode->key) {
			//No childs:
			if (currentNode->left == nullptr && currentNode->right == nullptr) {
				//If target node is root
				if (previousNode == nullptr) {
					root = nullptr;
				}
				else {

					if (previousNode->left == currentNode)
						previousNode->left = nullptr;
					else
						previousNode->right = nullptr;

					delete currentNode;
					--size_;
					return true;
				}
			}
			//Both childs exist
			if (currentNode->left != nullptr && currentNode->right != nullptr) {
				Node* minimumInRightBranch = currentNode->right;
				Node* minimumInRightBranchPrev = currentNode;
				//Finding minimum in the right branch
				while (minimumInRightBranch->left != nullptr) {
					minimumInRightBranchPrev = minimumInRightBranch;
					minimumInRightBranch = minimumInRightBranch->left;
				}
				//If minimum node has right child -> replace it on minimum node place
				if (minimumInRightBranch->right != nullptr)
					minimumInRightBranchPrev->right = minimumInRightBranch->right;
				else {
					if (minimumInRightBranchPrev->left == minimumInRightBranch)
						minimumInRightBranchPrev->left = nullptr;
					else
						minimumInRightBranchPrev->right = nullptr;
				}
				//Replace minimum node on target node place
				if (previousNode == nullptr) {
					root = minimumInRightBranch;
				}
				else {
					if (previousNode->left == currentNode)
						previousNode->left = minimumInRightBranch;
					else
						previousNode->right = minimumInRightBranch;
				}
				minimumInRightBranch->left = currentNode->left;
				if (minimumInRightBranch != currentNode->right)
					minimumInRightBranch->right = currentNode->right;
				delete currentNode;
				--size_;
				return true;
			}
			//If there is only left child
			if (currentNode->left != nullptr) {
				if (previousNode != nullptr) {
					if (previousNode->left == currentNode)
						previousNode->left = currentNode->left;
					else
						previousNode->right = currentNode->left;
				}
				else {
					root = currentNode->left;
				}
			}
			//If there is only right child
			else {
				if (previousNode != nullptr) {
					if (previousNode->left == currentNode)
						previousNode->left = currentNode->right;
					else
						previousNode->right = currentNode->right;
				}
				else {
					root = currentNode->right;
				}
			}
			delete currentNode;
			--size_;
			return true;
		}
		// Key is not equal -> continue search
		else {
			previousNode = currentNode;
			if (key < currentNode->key) {
				if (currentNode->left != nullptr) {
					currentNode = currentNode->left;
				}
				else {
					return false;
				}
			}
			else {
				if (currentNode->right != nullptr) {
					currentNode = currentNode->right;
				}
				else {
					return false;
				}
			}
		}
	}
}

template<Comparable K, CopyConstructible V>
inline V& BinaryTree<K, V>::operator[](const K& key)
{

	Node* currentNode = root;

	if (currentNode == nullptr) {
		Node* newNode = new Node();
		newNode->key = key;
		root = newNode;
		size_++;
		return root->value;
	}

	while (true) {
		if (key == currentNode->key) {
			return currentNode->value;
		}
		if (key > currentNode->key) {
			if (currentNode->right == nullptr) {
				Node* newNode = new Node();
				newNode->key = key;
				currentNode->right = newNode;
				return currentNode->right->value;
				break;
			}
			else {
				currentNode = currentNode->right;
				continue;
			}
		}
		if (key < currentNode->key) {
			if (currentNode->left == nullptr) {
				Node* newNode = new Node();
				newNode->key = key;
				currentNode->left = newNode;
				return currentNode->left->value;
				break;
			}
			else {
				currentNode = currentNode->left;
				continue;
			}
		}
	}
	size_++;
}

template<Comparable K, CopyConstructible V>
BinaryTree<K, V>::iterator BinaryTree<K, V>::find(const K& key) {
	Node* currentNode = root;
	if (currentNode == nullptr) return iterator(nullptr);
	while (currentNode->key != key) {
		if (key < currentNode->key)
			if (currentNode->left != nullptr)
				currentNode = currentNode->left;
			else
				return iterator(nullptr);
		if (key > currentNode->key)
			if (currentNode->right != nullptr)
				currentNode = currentNode->right;
			else
				return iterator(nullptr);
	}
	return iterator(currentNode);
}


template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::iterator BinaryTree<K, V>::end()
{
	if (this->root == nullptr) return iterator();
	iterator a;
	Node* currentNode = this->root;
	while (currentNode) {
		a.nodes.push(currentNode);
		currentNode = currentNode->right;
	}
	a.ptr = currentNode;
	return a;
}

template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::const_iterator BinaryTree<K, V>::cend() const
{
	if (this->root == nullptr) return iterator();
	iterator a;
	Node* currentNode = this->root;
	while (currentNode) {
		a.nodes.push(currentNode);
		currentNode = currentNode->right;
	}
	a.ptr = currentNode;
	return a;
}

template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::iterator BinaryTree<K, V>::begin()
{
	if (this->root == nullptr) return iterator();
	iterator a;
	Node* currentNode = this->root;
	while (currentNode->left) {
		a.nodes.push(currentNode);
		currentNode = currentNode->left;
	}
	a.ptr = currentNode;
	return a;
}
template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::const_iterator BinaryTree<K, V>::cbegin() const
{
	const_iterator a;
	Node* currentNode = this->root;
	while (currentNode->left) {
		a.nodes.push(currentNode);
		currentNode = currentNode->left;
	}
	a.ptr = currentNode;
	return a;

}

template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::reverse_iterator BinaryTree<K, V>::rend()
{
	if (this->root == nullptr) return reverse_iterator();
	reverse_iterator a;
	Node* currentNode = this->root;
	while (currentNode) {
		a.nodes.push(currentNode);
		currentNode = currentNode->left;
	}
	a.ptr = currentNode;
	return a;
}

template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::const_reverse_iterator BinaryTree<K, V>::crend() const
{
	if (this->root == nullptr) return reverse_iterator();
	reverse_iterator a;
	Node* currentNode = this->root;
	while (currentNode) {
		a.nodes.push(currentNode);
		currentNode = currentNode->left;
	}
	a.ptr = currentNode;
	return a;
}

template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::reverse_iterator BinaryTree<K, V>::rbegin()
{
	if (this->root == nullptr) return reverse_iterator();
	reverse_iterator a;
	Node* currentNode = this->root;
	while (currentNode->right) {
		a.nodes.push(currentNode);
		currentNode = currentNode->right;
	}
	a.ptr = currentNode;
	return a;
}

template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::const_reverse_iterator BinaryTree<K, V>::crbegin() const
{
	if (this->root == nullptr) return reverse_iterator();
	reverse_iterator a;
	Node* currentNode = this->root;
	while (currentNode->right) {
		a.nodes.push(currentNode);
		currentNode = currentNode->right;
	}
	a.ptr = currentNode;
	return a;
}

template<Comparable K, CopyConstructible V>
inline bool BinaryTree<K, V>::insert(const K& key, const V& value)
{
	Node* currentNode = root;

	if (currentNode == nullptr) {
		Node* newNode = new Node();
		newNode->key = key;
		newNode->value = value;
		root = newNode;
		size_++;
		return true;
	}

	while (true) {
		if (key == currentNode->key) return false;
		if (key > currentNode->key) {
			if (currentNode->right == nullptr) {
				Node* newNode = new Node();
				newNode->key = key;
				newNode->value = value;
				currentNode->right = newNode;
				break;
			}
			else {
				currentNode = currentNode->right;
				continue;
			}
		}
		if (key < currentNode->key) {
			if (currentNode->left == nullptr) {
				Node* newNode = new Node();
				newNode->key = key;
				newNode->value = value;
				currentNode->left = newNode;
				break;
			}
			else {
				currentNode = currentNode->left;
				continue;
			}
		}
	}
	size_++;
	return true;
}
*/