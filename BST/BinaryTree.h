#pragma once
#include <concepts>
#include <string>
#include <functional>
#include <stack>
#include <queue>
#include <math.h>

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



template <Comparable K, CopyConstructible V>
class BinaryTree
{
private:

	struct Node {
		Node* parent;
		K key;
		V value;
		Node* left = nullptr;
		Node* right = nullptr;
	};

	Node* root = nullptr;
	size_t size_ = 0;

	void forEach(std::function<void(Node*)>);

public:
	class const_iterator;
	class iterator;
	class reverse_iterator;
	class const_reverse_iterator;
	
	/*============= Rule of five ===============*/

	BinaryTree() {};
	BinaryTree(const BinaryTree& other);
	BinaryTree(BinaryTree&& other);
	BinaryTree& operator=(const BinaryTree& other);
	BinaryTree& operator=(BinaryTree&& other);

	/*============= Informational ===============*/

	// Returns true or false if tree contains provided key
	bool contains(const K& key) const;
	size_t size() const;
	bool empty() const;

	/*=============== Access ===================*/

	iterator find(const K& key);
	const_iterator find(const K& key) const;
	iterator end();
	const_iterator cend() const;
	iterator begin();
	const_iterator cbegin() const;
	reverse_iterator rend();
	const_reverse_iterator crend() const;
	reverse_iterator rbegin();
	const_reverse_iterator crbegin() const;
	V& operator[](const K&);
	V operator[](const K&) const;

	/*============= Operations =================*/

	bool insert(const K&, const V&);
	bool remove(const K&);
	void clear();
	void print();

	/*============= Pass through =============*/

	//In order iterative pass algorithm. Transfers every tree value into lambda argument
	void forEach(std::function<void(const K&, V&)>);
	//In order iterative pass algotithm. Transfers every tree key and corresponding value into lambda argument
	void forEach(std::function<void(const K&, const V&)>) const;
	//Horizontal iterative pass algorithm. Transfers every tree key and corresponding value into lambda argument
	void forEachHorizontal(std::function<void(const K&, V&, size_t)>);
	//Horizontal iterative pass algotithm. Transfers every tree key and corresponding value into lambda argument
	void forEachHorizontal(std::function<void(const K&, const V&, size_t)>) const;


	/*============= Destructor ==============*/

	~BinaryTree();

	/*============= Iterators ===============*/


	class iterator_base {
	protected:
		friend class BinaryTree;
		BinaryTree<K, V>::Node* ptr = nullptr;
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

	class forward_iterator_base : public iterator_base {
	protected:
		forward_iterator_base(Node* node) : iterator_base(node) {};
		void goForward() override;
		void goBackward() override;
		Node* endNodePrevious = nullptr;
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

	class reverse_iterator_base : public iterator_base {
	protected:
		reverse_iterator_base(Node* node) : iterator_base(node) {};
		void goForward() override;
		void goBackward() override;
		Node* endNodePrevious = nullptr;
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

	//Constant forward iterator. Doesn't allow to change the value while passing through
	class const_iterator : public forward_iterator_base {
	private:
		friend class BinaryTree;
		const_iterator(Node* node) : forward_iterator_base(node) {};
	public:
		const_iterator() = default;
		const_iterator(const iterator& other) { this->copy((iterator_base&)other); };
		const_iterator(const const_iterator& other) = default;
		std::pair<const K&, V> operator*() const { return this->get(); };
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

	//Constant reverse iterator. Provides backward going through the container. Doesn't allow to change the value while passing through.
	class const_reverse_iterator : public reverse_iterator_base {
	private:
		friend class BinaryTree;
		const_reverse_iterator(Node* node) : reverse_iterator_base(node) {};
	public:
		const_reverse_iterator() = default;
		const_reverse_iterator(const const_reverse_iterator& other) = default;
		const_reverse_iterator(const reverse_iterator& other) { this->copy((iterator_base&)other); };
		std::pair<const K&, V> operator*() const { return this->get(); };
		const_reverse_iterator& operator++() { this->goForward(); return *this; };
		const_reverse_iterator operator++(int) {const_reverse_iterator newVal = *this; this->goForward(); return newVal; };
		const_reverse_iterator& operator--() { this->goBackward(); return *this; };
		const_reverse_iterator operator--(int) { const_reverse_iterator newVal = *this; this->goBackward(); return newVal; };
	};

	//Reverse iterator. Provides backward going through the container.
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
	/*=======================================*/
};

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
BinaryTree<K, V>::const_iterator BinaryTree<K, V>::find(const K& key) const {
	Node* currentNode = root;
	if (currentNode == nullptr) return const_iterator(nullptr);
	while (currentNode->key != key) {
		if (key < currentNode->key)
			if (currentNode->left != nullptr)
				currentNode = currentNode->left;
			else
				return false;
		if (key > currentNode->key)
			if (currentNode->right != nullptr)
				currentNode = currentNode->right;
			else
				return false;
	}
	return const_iterator(currentNode);
}

template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::iterator BinaryTree<K, V>::end()
{
	if (root == nullptr) return iterator(nullptr);
	iterator it(nullptr);
	Node* endNodePrevious = root;
	while (endNodePrevious->right)
		endNodePrevious = endNodePrevious->right;
	it.endNodePrevious = endNodePrevious;
	return it;
}

template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::const_iterator BinaryTree<K, V>::cend() const
{
	if (root == nullptr) return const_iterator(nullptr);
	const_iterator it(nullptr);
	Node* endNodePrevious = root;
	while (endNodePrevious->right)
		endNodePrevious = endNodePrevious->right;
	it.endNodePrevious = endNodePrevious;
	return it;
}

template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::iterator BinaryTree<K, V>::begin()
{
	if (root == nullptr) return iterator(nullptr);
	Node* currentNode = root;
	while (currentNode->left)
		currentNode = currentNode->left;
	return iterator(currentNode);
}
template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::const_iterator BinaryTree<K, V>::cbegin() const
{
	if (root == nullptr) return const_iterator(nullptr);
	Node* currentNode = root;
	while (currentNode->left)
		currentNode = currentNode->left;
	return const_iterator(currentNode);
}

template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::reverse_iterator BinaryTree<K, V>::rend()
{
	if (root == nullptr) return reverse_iterator(nullptr);
	reverse_iterator it(nullptr);
	Node* endNodePrevious = root;
	while (endNodePrevious->left)
		endNodePrevious = endNodePrevious->left;
	it.endNodePrevious = endNodePrevious;
	return it;
}

template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::const_reverse_iterator BinaryTree<K, V>::crend() const
{
	if (root == nullptr) return const_reverse_iterator(nullptr);
	const_reverse_iterator it(nullptr);
	Node* endNodePrevious = root;
	while (endNodePrevious->left)
		endNodePrevious = endNodePrevious->left;
	it.endNodePrevious = endNodePrevious;
	return it;
}

template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::reverse_iterator BinaryTree<K, V>::rbegin()
{
	if (root == nullptr) return reverse_iterator(nullptr);
	Node* currentNode = root;
	while (currentNode->right)
		currentNode = currentNode->right;
	return reverse_iterator(currentNode);
}

template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::const_reverse_iterator BinaryTree<K, V>::crbegin() const
{
	if (root == nullptr) return reverse_iterator(nullptr);
	Node* currentNode = root;
	while (currentNode->right)
		currentNode = root->right;
	return const_reverse_iterator(currentNode);
}

template<Comparable K, CopyConstructible V>
inline V& BinaryTree<K, V>::operator[](const K& key)
{
	auto it = find(key);
	if (it != end())
		return (*it).second;
	else throw std::out_of_range("No such key in the tree");

}

template<Comparable K, CopyConstructible V>
inline V BinaryTree<K, V>::operator[](const K& key) const
{
	auto it = find(key);
	if (it != cend())
		return (*it).second;
	else throw std::out_of_range("No such key in the tree");
}

template<Comparable K, CopyConstructible V>
bool BinaryTree<K, V>::empty() const {
	return size_ == 0;
}

template<Comparable K, CopyConstructible V>
size_t BinaryTree<K, V>::size() const {
	return size_;
}

template<Comparable K, CopyConstructible V>
bool BinaryTree<K, V>::contains(const K& key) const {
	Node* currentNode = root;
	if (currentNode == nullptr) return false;
	while (currentNode->key != key) {
		if (key < currentNode->key)
			if (currentNode->left != nullptr)
				currentNode = currentNode->left;
			else
				return false;
		if (key > currentNode->key)
			if (currentNode->right != nullptr)
				currentNode = currentNode->right;
			else
				return false;
	}
	return true;
}


template<Comparable K, CopyConstructible V>
inline void BinaryTree<K, V>::forEach(std::function<void(typename BinaryTree<K,V>::Node*)> func) {
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
inline bool BinaryTree<K, V>::insert(const K& key, const V& value)
{
	Node* currentNode = root;
	Node* newNode = new Node();
	newNode->key = key;
	newNode->value = value;

	if (currentNode == nullptr) {
		root = newNode;
		size_++;
		return true;
	}

	while (true) {
		if (key == currentNode->key) return false;
		if (key > currentNode->key) {
			if (currentNode->right == nullptr) {
				currentNode->right = newNode;
				newNode->parent = currentNode;
				break;
			}
			else {
				currentNode = currentNode->right;
				continue;
			}
		}
		if (key < currentNode->key) {
			if (currentNode->left == nullptr) {
				currentNode->left = newNode;
				newNode->parent = currentNode;
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

template<Comparable K, CopyConstructible V>
inline bool BinaryTree<K, V>::remove(const K& key)
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
				if (minimumInRightBranch->right != nullptr) minimumInRightBranchPrev->left = minimumInRightBranch->right;
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
inline void BinaryTree<K, V>::clear()
{
	std::stack<Node*> toDelete;
	forEach([&](Node* node) {
		toDelete.push(node);
	});
	
	while (!toDelete.empty()) {
		delete toDelete.top();
		toDelete.pop();
	}
	root = nullptr;
	size_ = 0;
}

template<Comparable K, CopyConstructible V>
inline void BinaryTree<K, V>::print()
{
	//создаем массив на количество элементов в зависимости от уровня.
	//сохраняем глубину и порядковый номер элемента на уровне!!!

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
	for (int i = 0; i < pow(2,MAXIMUM_LEVEL); i++) {
		std::vector<Node*> innerVector;
		for (int j = 0; j < pow(2,MAXIMUM_LEVEL); j++) {
			innerVector.push_back(nullptr);
		}
		vectors.push_back(innerVector);
	}

	size_t maxDepth;
	std::queue<triplet> nodes;
	do {
		if (!nodes.empty()) nodes.pop();
		vectors[depth][ordinalNumber] = root;
		if (root->left != nullptr) nodes.push(triplet(root->left, depth+1, ordinalNumber*2));
		if (root->right != nullptr) nodes.push(triplet(root->right, depth+1, ordinalNumber*2+1));
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
	int symbolsInString = maxKeySize * maxValuesNumber + maxValuesNumber; //числа (3 символа) + пробелы (1 символ)
	for (int i = 0; i <= maxDepth && i<=MAXIMUM_LEVEL; i++) {
		int thisStringValuesNumber = pow(2, i);
		for (int j = 0; j < thisStringValuesNumber; j++) {
			int probelsAfterEveryKey = (symbolsInString - (thisStringValuesNumber * maxKeySize)) / thisStringValuesNumber/2;
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

template<Comparable K, CopyConstructible V>
inline void BinaryTree<K, V>::forEach(std::function<void(const K&, V&)> func) {
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
inline void BinaryTree<K, V>::forEach(std::function<void(const K&, const V&)> func) const {
	Node* root = root;
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
inline void BinaryTree<K, V>::forEachHorizontal(std::function<void(const K&, V&, size_t)> func) {
	
	if (this->root == nullptr) return;
	Node* root = this->root;
	size_t depth = 0;
	std::queue<std::pair<Node*,size_t>> nodes;
	do {
		func(root->key, root->value, depth);
		if (root->left != nullptr) nodes.push(std::make_pair(root->left,depth+1));
		if (root->right != nullptr) nodes.push(std::make_pair(root->right,depth+1));
		if (!nodes.empty()) {
			root = nodes.front().first;
			depth = nodes.front().second;
			nodes.pop();
		}
	} while (!nodes.empty());
}

template<Comparable K, CopyConstructible V>
inline void BinaryTree<K, V>::forEachHorizontal(std::function<void(const K&, const V&, size_t)> func) const {
	if (this->root == nullptr) return;
	Node* root = this->root;
	size_t depth = 0;
	std::queue<std::pair<Node*, size_t>> nodes;
	do {
		func(root->key, root->value, depth);
		if (root->left != nullptr) nodes.push(std::make_pair(root->left, depth + 1));
		if (root->right != nullptr) nodes.push(std::make_pair(root->right, depth + 1));
		if (!nodes.empty()) {
			root = nodes.front().first;
			depth = nodes.front().second;
			nodes.pop();
		}
	} while (!nodes.empty());
}




template<Comparable K, CopyConstructible V>
inline BinaryTree<K, V>::BinaryTree(const BinaryTree<K,V>& other)
{
	other.forEach([&](const K& key, const V& val) {
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
inline BinaryTree<K, V>::~BinaryTree()
{
	clear();
}

template<Comparable K, CopyConstructible V>
inline BinaryTree<K,V>& BinaryTree<K, V>::operator=(const BinaryTree& other)
{
	if (this != &other) {
		this->clear();
		other.forEach([&](const K& key, const V& val){
			this->insert(key, val);
		});
	}
	return *this;
}

template<Comparable K, CopyConstructible V>
inline BinaryTree<K,V>& BinaryTree<K, V>::operator=(BinaryTree&& other)
{
	if (this != &other) {
		this->root = other.root;
		this->size_ = other.size_;
		other.root = nullptr;
	}
	return *this;
}
/*
template<Comparable K, CopyConstructible V>
bool operator==(const typename BinaryTree<K,V>::iterator_base& one, const typename BinaryTree<K, V>::iterator_base& two) { return true; };

template<Comparable K, CopyConstructible V>
std::strong_ordering operator<=>(const typename BinaryTree<K, V>::reverse_iterator_base& one, const typename BinaryTree<K, V>::reverse_iterator_base& two) { return std::strong_ordering::equal; };
*/

template<Comparable K, CopyConstructible V>
inline void BinaryTree<K, V>::forward_iterator_base::goForward()
{
	if (this->ptr == nullptr) {
		if(this->endNodePrevious == nullptr)
			throw std::logic_error("Iterator going forward operation: can't use the iterator isn't linked to any container");
		else
			throw std::logic_error("Iterator going forward operation: can't go through the end element.");
	}
	// If there are any children, bigger then current:
	if (this->ptr->right) {
		this->ptr = this->ptr->right;
		//if bigger child has children less then it -> go through the left branch
		while (this->ptr->left)
			this->ptr = this->ptr->left;
		return;
	}
	// If there are no bigger children -> go to the first parent node with the bigger key:
	Node* temp = this->ptr;
	while ((temp = temp->parent)) {
		if (temp->key > this->ptr->key) break;
	}
	// If that parent exists
	if (temp != nullptr) {
		this->ptr = temp;
	}
	//Else -> go to the endNode
	else {
		this->endNodePrevious = this->ptr;
		this->ptr = nullptr;
	}
}

template<Comparable K, CopyConstructible V>
inline void BinaryTree<K, V>::forward_iterator_base::goBackward()
{
	if (this->ptr == nullptr) {
		if (this->endNodePrevious != nullptr) {
			this->ptr = this->endNodePrevious;
			return;
		}
		else {
			throw std::logic_error("Iterator going back operation: can't use the iterator isn't linked to any container");
		}
	}

	// If there are any children, less then current:
	if (this->ptr->left) {
		this->ptr = this->ptr->left;
		//if less child has children bigger then it -> go through the right branch
		while (this->ptr->right)
			this->ptr = this->ptr->right;
		return;
	}
	// If there are no less children -> go to the first parent node with the less key:
	Node* temp = this->ptr;
	while ((temp = temp->parent)) {
		if (temp->key < this->ptr->key) break;
	}
	// If that parent exists
	if (temp != nullptr) {
		this->ptr = temp;
	}
	//Else -> we are at the begin element
	else {
		throw std::logic_error("Iterator going back operation: can't go through the begin element");
	}
}

template<Comparable K, CopyConstructible V>
inline void BinaryTree<K, V>::reverse_iterator_base::goForward()
{
	if (this->ptr == nullptr) {
		if (this->endNodePrevious == nullptr)
			throw std::logic_error("Reverse Iterator going forward operation: can't use the iterator isn't linked to any container");
		else
			throw std::logic_error("Reverse Iterator going forward operation: can't go through the rend element.");
	}

	// If there are any children, less then current:
	if (this->ptr->left) {
		this->ptr = this->ptr->left;
		//if less child has children bigger then it -> go through the right branch
		while (this->ptr->right)
			this->ptr = this->ptr->right;
		return;
	}
	// If there are no less children -> go to the first parent node with the less key:
	Node* temp = this->ptr;
	while ((temp = temp->parent)) {
		if (temp->key < this->ptr->key) break;
	}
	// If that parent exists
	if (temp != nullptr) {
		this->ptr = temp;
	}
	//Else -> go to the endNode
	else {
		this->endNodePrevious = this->ptr;
		this->ptr = nullptr;
	}
}

template<Comparable K, CopyConstructible V>
inline void BinaryTree<K, V>::reverse_iterator_base::goBackward()
{
	if (this->ptr == nullptr) {
		if (this->endNodePrevious != nullptr) {
			this->ptr = this->endNodePrevious;
			return;
		}
		else {
			throw std::logic_error("Iterator going back operation: can't use the iterator isn't linked to any container");
		}
	}
	// If there are any children, bigger then current:
	if (this->ptr->right) {
		this->ptr = this->ptr->right;
		//if bigger child has children less then it -> go through the left branch
		while (this->ptr->left)
			this->ptr = this->ptr->left;
		return;
	}
	// If there are no bigger children -> go to the first parent node with the bigger key:
	Node* temp = this->ptr;
	while ((temp = temp->parent)) {
		if (temp->key > this->ptr->key) break;
	}
	// If that parent exists
	if (temp != nullptr) {
		this->ptr = temp;
	}
	//Else -> we are at the begin element
	else {
		throw std::logic_error("Iterator going back operation: can't go through the rbegin element");
	}
}
