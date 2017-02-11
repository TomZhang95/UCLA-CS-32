// Sequence.cpp
#include <iostream>
#include "Sequence.h"

Sequence::Sequence()
: m_size(0)
{
    m_head = nullptr;
}

bool Sequence::insert(int pos, const ItemType& value)
{
    Node* n = new Node;
    Node* temp = m_head;
	if (pos < 0)
		return false;
	else
	{
		if (m_head == nullptr)
		{
			n->m_data = value;
			n->m_next = n;
			n->m_prev = n;
			m_head = n;
			
			m_size++;
		}
		else
		{
			if (pos >= m_size)
			{
				n->m_data = value;
				n->m_prev = m_head->m_prev;
				n->m_next = m_head;
				m_head->m_prev->m_next = n;
				m_head->m_prev = n;
				m_size++;
			}
			else
			{
				for (int i = 0; i < pos; i++)
					temp = temp->m_next;
				n->m_data = value;
				n->m_next = temp;
				n->m_prev = temp->m_prev;
				temp->m_prev->m_next = n;
				temp->m_prev = n;
				m_size++;
				if (pos == 0)
					m_head = n;
			}
		}
		return true;
	}
}

int Sequence::insert(const ItemType& value)
{ 
    Node* temp = m_head;
    int pos = m_size + 1;
	for (int i = 0; i < m_size; i++)
	{
		if (temp->m_data == value)
		{
			pos = i;
			break;
		}
		temp = temp->m_next;
	}
        
    if (pos != m_size + 1)
    {
        insert(pos, value);
        return pos;
    }
    else
    {
        insert(m_size, value);
        return m_size;
    }
    
}

bool Sequence::erase(int pos)
{
    if (pos < 0  ||  pos >= size())
        return false;
	else
	{
		Node*temp = m_head;
		for (int i = 0; i < pos; i++)
			temp = temp->m_next;
		temp->m_prev->m_next = temp->m_next;
		temp->m_next->m_prev = temp->m_prev;
		if (pos == 0)
			m_head = m_head->m_next;
		delete temp;
		m_size--;
		return true;
	}
 
}

int Sequence::remove(const ItemType& value)
{
	int count = 0;
	Node* temp = m_head;
	for (int i = 0; i < m_size; i++)
	{
		if (temp->m_data == value)
		{
			if (i == 0)
				m_head = m_head->m_next;
			temp->m_prev->m_next = temp->m_next;
			temp->m_next->m_prev = temp->m_prev;
			count++;
			delete temp;
			m_size--;
		}
		temp = temp->m_next;
	}
	return count;
}

bool Sequence::get(int pos, ItemType& value) const
{
    if (pos < 0  ||  pos >= size())
        return false;
	else
	{
		Node*temp = m_head;
		for (int i = 0; i < pos; i++)
			temp = temp->m_next;
		value = temp->m_data;
		return true;
	}
}

bool Sequence::set(int pos, const ItemType& value)
{
    if (pos < 0  ||  pos >= size())
        return false;
	else
	{
		Node* temp = m_head;
		for (int i = 0; i < pos; i++)
			temp = temp->m_next;
		temp->m_data = value;
		return true;
	}
}

int Sequence::find(const ItemType& value) const
{
	Node* temp = m_head;
	for (int pos = 0; pos < size(); pos++)
	{
		if (temp->m_data == value)
			return pos;
		temp = temp->m_next;
	}
    return -1;
}

void Sequence::swap(Sequence& other)
{
	Node* temp = m_head;
	int s = m_size;
	
	m_head = other.m_head;
	other.m_head = temp;

	m_size = other.m_size;
	other.m_size = s;

}

Sequence::Sequence(const Sequence & other)
{
	ItemType data;
	for (int i = 0; i < m_size; i++)
	{
		other.get(i, data);
		insert(i, data);
	}
}

Sequence & Sequence::operator=(const Sequence & rhs)
{
	if (this != &rhs)
	{
		Sequence temp(rhs);
		swap(temp);
	}
	return *this;
}

Sequence::~Sequence()
{
	Node* temp = m_head;
	if(temp != nullptr)
		while (temp->m_next != m_head)
		{
			temp = temp->m_next;
			delete temp->m_prev;
		}
	delete temp;
}

int subsequence(const Sequence & seq1, const Sequence & seq2)
{
	int count = 1;
	ItemType data1, data2;
	if (seq2.empty())
		return -1;
	for (int j = 0; j < seq1.size(); j++)
	{
		seq2.get(0, data2);
		seq1.get(j, data1);
		if (data1 == data2)
		{
			if (seq2.size() == 1)
				return j;
			else
			{
				for (int m =1; m < seq2.size(); m++)
				{
					seq1.get(m+j, data1);
					seq2.get(m, data2);
					if (data1 == data2)
						count++;
				}
				if (count == seq2.size())
					return j;
				else
					count = 1;
			}

		}
	}
	return -1;
}

void interleave(const Sequence & seq1, const Sequence & seq2, Sequence & result)
{
	ItemType data1, data2;
	int size = seq1.size();
	if (size < seq2.size())
		size = seq2.size();
	for (int i = 0; i < size; i++)
	{
		if (!seq1.empty() && i < seq1.size())
		{
			seq1.get(i, data1);
			result.insert(result.size(), data1);
		}
		if (!seq2.empty() && i < seq2.size())
		{
			seq2.get(i, data2);
			result.insert(result.size(), data2);
		}
	}
}
