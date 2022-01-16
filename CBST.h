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
	tBSTNode<T1, T2>* m_pRoot; //루트  노드 주소
	int m_iCount; //노드 개수
	 
public:
	bool insert(const tPair<T1, T2>& _pair);
	tBSTNode<T1, T2>* GetInorderSuccessor(tBSTNode<T1, T2>* _pNode);
	tBSTNode<T1, T2>* GetInorderPredecessor(tBSTNode<T1, T2>* _pNode);

	class iterator;
public:
	iterator begin();
	iterator end();
	iterator find(const T1& _find);//key값을 인자로 받음
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
		tBSTNode<T1, T2>* m_pNode; //null인 경우 end iterator
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
		const tPair<T1, T2>* operator ->() //->자체는 주소에 접근하는 방식. 
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

	//첫번째  데이터 라면.
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
				pNode = pNode->arrNode[(int)node_type]; //root가 아닌 오른쪽을 가리킴
			}
		}

	}

	//데이터 증가
	m_iCount++;
	return true;
}
template<typename T1, typename T2>
inline tBSTNode<T1, T2>* CBST<T1, T2>::GetInorderSuccessor(tBSTNode<T1, T2>* _pNode)
{
	tBSTNode<T1, T2>* pSuccessor = nullptr;

	//1. 오른쪽 자식이 있는 경우 : 왼쪽 자식이 없을떄까지 내려감.
	if (_pNode->arrNode[(int)NODE_TYPE::RCHILD] != nullptr)
	{
		pSuccessor = _pNode->arrNode[(int)NODE_TYPE::RCHILD];
		while (pSuccessor->arrNode[(int)NODE_TYPE::LCHILD])
		{
			pSuccessor = pSuccessor->arrNode[(int)NODE_TYPE::LCHILD];
		}
	}
	//2. 부모로 부터 왼쪽 자식일떄까지 올라감.
	else
	{
		pSuccessor = _pNode;
		while (true)
		{
			//더 이상 위쪽으로 못 올라갈 떄
			if (pSuccessor->IsRoot())
			{
				return nullptr;
			}
			//부모 부터 왼쪽 자식인지 체크
			if (pSuccessor->IsLeftChild())
			{
				//그떄 부모가 후속자
				pSuccessor = pSuccessor->arrNode[(int)NODE_TYPE::PARENT];
				break;
			}
			else
			{
				//왼쪽 자식이 아니라면 부모 노드로 점프.
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
	//1. 왼쪽 노드가 있을 경우 : (오른쪽 자식이 없을떄까지 내려감)
	if (_pNode->arrNode[(int)NODE_TYPE::LCHILD] != nullptr)
	{
		pSuccessor = _pNode->arrNode[(int)NODE_TYPE::LCHILD];
		while (pSuccessor->arrNode[(int)NODE_TYPE::RCHILD])
		{
			pSuccessor = pSuccessor->arrNode[(int)NODE_TYPE::RCHILD];
		}
	}
	//2. 부모로부터 오른쪽 자식일떄까지 올라감.
	else
	{
		pSuccessor = _pNode;
		while (true)
		{
			//더 이상 위쪽으로 못 올라갈 떄
			if (pSuccessor->IsRoot())
			{
				return nullptr;
			}
			//부모로 부터 오른쪽 자식인지 체크
			if (pSuccessor->IsRightChild())
			{
				//그떄 부모가 후속자
				pSuccessor = pSuccessor->arrNode[(int)NODE_TYPE::PARENT];
				break;
			}
			else
			{
				//왼쪽 자식이 아니라면 부모 노드로 점프.
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
			//찾았다.
			break;
		}

		if (pNode->arrNode[(int)node_type] == nullptr)
		{
			//못찾았다.
			return end();
		}
		else
		{
			pNode = pNode->arrNode[(int)node_type]; //root가 아닌 오른쪽을 가리킴
		}
	}
	return iterator(this, pNode);
}




template<typename T1, typename T2>
inline typename CBST<T1, T2>::iterator CBST<T1, T2>::erase(const iterator& _iter)
{
	assert(this == _iter.m_pBST); //같다면 true : 그냥 지나감.

	tBSTNode<T1, T2>* pSccessor = DeleteNode(_iter.m_pNode);

	return iterator(this, pSccessor);
	
}

template<typename T1, typename T2>
inline tBSTNode<T1, T2>* CBST<T1, T2>::DeleteNode(tBSTNode<T1, T2>* _pTargetNode)
{
	tBSTNode<T1, T2>* pSccessor = GetInorderSuccessor(_pTargetNode);
	//자식이 아예 없는가?
	if (_pTargetNode->IsLeaf())
	{

		if (_pTargetNode == m_pRoot)
		{
			m_pRoot = nullptr;
		}
		else
		{
			//부모노드로 접근, 삭제될 노드의 자식을 가리키는 포인터를 nullptr로 만든다.
			if (_pTargetNode->IsLeftChild())
			{
				_pTargetNode->arrNode[(int)NODE_TYPE::PARENT]->arrNode[(int)NODE_TYPE::LCHILD] = nullptr;
			}
			else
			{
				_pTargetNode->arrNode[(int)NODE_TYPE::PARENT]->arrNode[(int)NODE_TYPE::RCHILD] = nullptr;
			}
		}
		delete _pTargetNode; //Node 동적 할당 해제
		--m_iCount;
		//return pSccessor;
	}

	//자식이 2개 다 있는가?
	else if (_pTargetNode->IsFull())
	{
		//삭제 될 자리에 중위 후속자의 값을 복사한다.
		_pTargetNode->pair = pSccessor->pair;
		
		DeleteNode(pSccessor);

		//리턴할 노드는, 복사된 노드값
		pSccessor = _pTargetNode;
	}

	//자식이 1개라도 있는경우
	else
	{
		//삭제 시킬 노드의 후속자 노드는 미리 찾아 둔다
		pSccessor = GetInorderSuccessor(_pTargetNode);

		NODE_TYPE eChildType = NODE_TYPE::LCHILD; //기본적으로 왼쪽 노드
		if (_pTargetNode->arrNode[(int)NODE_TYPE::RCHILD])
		{
			eChildType = NODE_TYPE::RCHILD; //만약 오른쪽 노드였다면, 오른쪽 노드로 타입 변경
		}

		if (_pTargetNode == m_pRoot)
		{
			m_pRoot = _pTargetNode->arrNode[(int)eChildType];
			_pTargetNode->arrNode[(int)eChildType]->arrNode[(int)NODE_TYPE::PARENT] = nullptr;
		}
		else 
		{
			//삭제될 노드의 부모와, 삭제될 노드의 자식을 연결해준다
			//부모로부터 왼쪽 노드인지, 오른쪽 노드인지 확인
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
