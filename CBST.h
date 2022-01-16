#pragma once
#include <assert.h>
enum class NODE_TYPE
{
	PARENT, //0
	LCHILD, //1
	RCHILD, //2
	END,    //3
};

template<typename T1, typename T2>
struct tPair
{
	T1 first;
	T2 second; 
};

template<typename T1, typename T2>
tPair<T1, T2> make_bstpair(const T1& _first, const T2& _second)
{
	return tPair<T1, T2>{_first, _second};
}


template<typename T1, typename T2>
struct tBSTNode
{
	//data
	tPair<T1, T2> pair;

	tBSTNode* arrNode[(int)NODE_TYPE::END];

	bool IsRoot()
	{
		if (arrNode[(int)NODE_TYPE::PARENT] == nullptr)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool IsLeftChild()
	{
		if (arrNode[(int)NODE_TYPE::PARENT]->arrNode[(int)NODE_TYPE::LCHILD] == this)
		{
			return true;
		}
		else
		{
			return false;
		}
		
		
	}
	bool IsRightChild()
	{
		if (arrNode[(int)NODE_TYPE::PARENT]->arrNode[(int)NODE_TYPE::RCHILD] == this)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool IsLeaf()
	{
		if (arrNode[(int)NODE_TYPE::LCHILD] == nullptr && arrNode[(int)NODE_TYPE::RCHILD] == nullptr)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool IsFull()
	{
		if (arrNode[(int)NODE_TYPE::LCHILD] && arrNode[(int)NODE_TYPE::RCHILD])
		{
			return true;
		}
		else
		{
			return false;
		}

	}


	tBSTNode()
		: pair()
		, arrNode{}
	{

	}
	tBSTNode(const tPair<T1, T2>& _pair, tBSTNode* _pParent, tBSTNode* _pLChild, tBSTNode* _pRChild)
		: pair(_pair)
		, arrNode{_pParent, _pLChild, _pRChild}
	{

	}

};
template<typename T1, typename T2> //key, value
class CBST
{
private:
	tBSTNode<T1, T2>* m_pRoot; //��Ʈ  ��� �ּ�
	int m_iCount; //��� ����
	 
public:
	bool insert(const tPair<T1, T2>& _pair);
	tBSTNode<T1, T2>* GetInorderSuccessor(tBSTNode<T1, T2>* _pNode);
	tBSTNode<T1, T2>* GetInorderPredecessor(tBSTNode<T1, T2>* _pNode);

	class iterator;
public:
	iterator begin();
	iterator end();
	iterator find(const T1& _find);//key���� ���ڷ� ����
	iterator erase(const iterator& _iter);
private:
	tBSTNode<T1, T2>* DeleteNode(tBSTNode<T1, T2>* _pTargetNode);
public:
	CBST()
		: m_pRoot(nullptr)
		, m_iCount(0)
	{
	}

	//iterator
	class iterator
	{
	private:
		CBST<T1, T2>* m_pBST;
		tBSTNode<T1, T2>* m_pNode; //null�� ��� end iterator
	public:
		bool operator ==(const iterator& _other)
		{
			if (m_pBST == _other.m_pBST && m_pNode == _other.m_pNode)
			{
				return true;
			}
			return false;
		}
		bool operator !=(const iterator& _other)
		{
			if (m_pBST == _other.m_pBST && m_pNode == _other.m_pNode)
			{
				return false;
			}
			return true;
		}
		const tPair<T1, T2>& operator *()
		{
			assert(m_pNode);
			return m_pNode->pair;
		}
		const tPair<T1, T2>* operator ->() //->��ü�� �ּҿ� �����ϴ� ���. 
		{
			assert(m_pNode);
			return &m_pNode->pair;
		}

		iterator& operator ++()
		{
			m_pNode = m_pBST->GetInorderSuccessor(m_pNode);
			return *this;
		}
		iterator& operator --()
		{
			m_pNode = m_pBST->GetInorderPredecessor(m_pNode);
			return *this;
		}



	public:
		iterator() : m_pBST(nullptr), m_pNode(nullptr)
		{

		}
		iterator(CBST<T1, T2>* _pBST, tBSTNode<T1, T2>* _pNode)
			: m_pBST(_pBST)
			, m_pNode(_pNode)
		{

		}
		friend class CBST<T1, T2>;

	};
};

template<typename T1, typename T2>
inline bool CBST<T1, T2>::insert(const tPair<T1, T2>& _pair)
{
	tBSTNode<T1, T2>* pNewNode = new tBSTNode<T1, T2>(_pair, nullptr, nullptr, nullptr);

	//ù��°  ������ ���.
	if (nullptr == m_pRoot)
	{
		m_pRoot = pNewNode;
	}
	else
	{
		tBSTNode<T1, T2>* pNode = m_pRoot;
		NODE_TYPE node_type = NODE_TYPE::END;
		while (true)
		{
			if (pNode->pair.first < pNewNode->pair.first)
			{
				node_type = NODE_TYPE::RCHILD;
			}
			else if (pNode->pair.first > pNewNode->pair.first)
			{
				node_type = NODE_TYPE::LCHILD;
			}
			else
			{
				return false;
			}

			if (pNode->arrNode[(int)node_type] == nullptr)
			{
				pNode->arrNode[(int)node_type] = pNewNode;
				pNewNode->arrNode[(int)NODE_TYPE::PARENT] = pNode;
				break;
			}
			else
			{
				pNode = pNode->arrNode[(int)node_type]; //root�� �ƴ� �������� ����Ŵ
			}
		}

	}

	//������ ����
	m_iCount++;
	return true;
}
template<typename T1, typename T2>
inline tBSTNode<T1, T2>* CBST<T1, T2>::GetInorderSuccessor(tBSTNode<T1, T2>* _pNode)
{
	tBSTNode<T1, T2>* pSuccessor = nullptr;

	//1. ������ �ڽ��� �ִ� ��� : ���� �ڽ��� ���������� ������.
	if (_pNode->arrNode[(int)NODE_TYPE::RCHILD] != nullptr)
	{
		pSuccessor = _pNode->arrNode[(int)NODE_TYPE::RCHILD];
		while (pSuccessor->arrNode[(int)NODE_TYPE::LCHILD])
		{
			pSuccessor = pSuccessor->arrNode[(int)NODE_TYPE::LCHILD];
		}
	}
	//2. �θ�� ���� ���� �ڽ��ϋ����� �ö�.
	else
	{
		pSuccessor = _pNode;
		while (true)
		{
			//�� �̻� �������� �� �ö� ��
			if (pSuccessor->IsRoot())
			{
				return nullptr;
			}
			//�θ� ���� ���� �ڽ����� üũ
			if (pSuccessor->IsLeftChild())
			{
				//�׋� �θ� �ļ���
				pSuccessor = pSuccessor->arrNode[(int)NODE_TYPE::PARENT];
				break;
			}
			else
			{
				//���� �ڽ��� �ƴ϶�� �θ� ���� ����.
				pSuccessor = pSuccessor->arrNode[(int)NODE_TYPE::PARENT];
			}
		}
	}
	return pSuccessor;
}


template<typename T1, typename T2>
inline tBSTNode<T1, T2>* CBST<T1, T2>::GetInorderPredecessor(tBSTNode<T1, T2>* _pNode)
{
	tBSTNode<T1, T2>* pSuccessor = nullptr;
	//1. ���� ��尡 ���� ��� : (������ �ڽ��� ���������� ������)
	if (_pNode->arrNode[(int)NODE_TYPE::LCHILD] != nullptr)
	{
		pSuccessor = _pNode->arrNode[(int)NODE_TYPE::LCHILD];
		while (pSuccessor->arrNode[(int)NODE_TYPE::RCHILD])
		{
			pSuccessor = pSuccessor->arrNode[(int)NODE_TYPE::RCHILD];
		}
	}
	//2. �θ�κ��� ������ �ڽ��ϋ����� �ö�.
	else
	{
		pSuccessor = _pNode;
		while (true)
		{
			//�� �̻� �������� �� �ö� ��
			if (pSuccessor->IsRoot())
			{
				return nullptr;
			}
			//�θ�� ���� ������ �ڽ����� üũ
			if (pSuccessor->IsRightChild())
			{
				//�׋� �θ� �ļ���
				pSuccessor = pSuccessor->arrNode[(int)NODE_TYPE::PARENT];
				break;
			}
			else
			{
				//���� �ڽ��� �ƴ϶�� �θ� ���� ����.
				pSuccessor = pSuccessor->arrNode[(int)NODE_TYPE::PARENT];
			}
		}
	}
	return pSuccessor;
}

template<typename T1, typename T2>
inline typename CBST<T1, T2>::iterator CBST<T1, T2>::begin()
{
	tBSTNode<T1, T2>* pNode = m_pRoot;
	while (pNode->arrNode[(int)NODE_TYPE::LCHILD])
	{
		pNode = pNode->arrNode[(int)NODE_TYPE::LCHILD];
	}
	return iterator(this, pNode);
}
template<typename T1, typename T2>
inline typename CBST<T1, T2>::iterator CBST<T1, T2>::end()
{
	return iterator(this, nullptr);
}
template<typename T1, typename T2>
inline typename CBST<T1, T2>::iterator CBST<T1, T2>::find(const T1& _find)
{
	tBSTNode<T1, T2>* pNode = m_pRoot;
	NODE_TYPE node_type = NODE_TYPE::END;
	while (true)
	{
		if (pNode->pair.first < _find)
		{
			node_type = NODE_TYPE::RCHILD;
		}
		else if (pNode->pair.first > _find)
		{
			node_type = NODE_TYPE::LCHILD;
		}
		else
		{
			//ã�Ҵ�.
			break;
		}

		if (pNode->arrNode[(int)node_type] == nullptr)
		{
			//��ã�Ҵ�.
			return end();
		}
		else
		{
			pNode = pNode->arrNode[(int)node_type]; //root�� �ƴ� �������� ����Ŵ
		}
	}
	return iterator(this, pNode);
}




template<typename T1, typename T2>
inline typename CBST<T1, T2>::iterator CBST<T1, T2>::erase(const iterator& _iter)
{
	assert(this == _iter.m_pBST); //���ٸ� true : �׳� ������.

	tBSTNode<T1, T2>* pSccessor = DeleteNode(_iter.m_pNode);

	return iterator(this, pSccessor);
	
}

template<typename T1, typename T2>
inline tBSTNode<T1, T2>* CBST<T1, T2>::DeleteNode(tBSTNode<T1, T2>* _pTargetNode)
{
	tBSTNode<T1, T2>* pSccessor = GetInorderSuccessor(_pTargetNode);
	//�ڽ��� �ƿ� ���°�?
	if (_pTargetNode->IsLeaf())
	{

		if (_pTargetNode == m_pRoot)
		{
			m_pRoot = nullptr;
		}
		else
		{
			//�θ���� ����, ������ ����� �ڽ��� ����Ű�� �����͸� nullptr�� �����.
			if (_pTargetNode->IsLeftChild())
			{
				_pTargetNode->arrNode[(int)NODE_TYPE::PARENT]->arrNode[(int)NODE_TYPE::LCHILD] = nullptr;
			}
			else
			{
				_pTargetNode->arrNode[(int)NODE_TYPE::PARENT]->arrNode[(int)NODE_TYPE::RCHILD] = nullptr;
			}
		}
		delete _pTargetNode; //Node ���� �Ҵ� ����
		--m_iCount;
		//return pSccessor;
	}

	//�ڽ��� 2�� �� �ִ°�?
	else if (_pTargetNode->IsFull())
	{
		//���� �� �ڸ��� ���� �ļ����� ���� �����Ѵ�.
		_pTargetNode->pair = pSccessor->pair;
		
		DeleteNode(pSccessor);

		//������ ����, ����� ��尪
		pSccessor = _pTargetNode;
	}

	//�ڽ��� 1���� �ִ°��
	else
	{
		//���� ��ų ����� �ļ��� ���� �̸� ã�� �д�
		pSccessor = GetInorderSuccessor(_pTargetNode);

		NODE_TYPE eChildType = NODE_TYPE::LCHILD; //�⺻������ ���� ���
		if (_pTargetNode->arrNode[(int)NODE_TYPE::RCHILD])
		{
			eChildType = NODE_TYPE::RCHILD; //���� ������ ��忴�ٸ�, ������ ���� Ÿ�� ����
		}

		if (_pTargetNode == m_pRoot)
		{
			m_pRoot = _pTargetNode->arrNode[(int)eChildType];
			_pTargetNode->arrNode[(int)eChildType]->arrNode[(int)NODE_TYPE::PARENT] = nullptr;
		}
		else 
		{
			//������ ����� �θ��, ������ ����� �ڽ��� �������ش�
			//�θ�κ��� ���� �������, ������ ������� Ȯ��
			if (_pTargetNode->IsLeftChild())
			{
				_pTargetNode->arrNode[(int)::NODE_TYPE::PARENT]->arrNode[(int)NODE_TYPE::LCHILD] = _pTargetNode->arrNode[(int)eChildType];
			}
			else
			{
				_pTargetNode->arrNode[(int)::NODE_TYPE::PARENT]->arrNode[(int)NODE_TYPE::RCHILD] = _pTargetNode->arrNode[(int)eChildType];
			}
			_pTargetNode->arrNode[(int)eChildType]->arrNode[(int)NODE_TYPE::PARENT] = _pTargetNode->arrNode[(int)NODE_TYPE::PARENT];

		}
		delete _pTargetNode;
		--m_iCount;
	}

	return pSccessor;

}
