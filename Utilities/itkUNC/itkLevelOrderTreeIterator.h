/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile$
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLevelOrderTreeIterator_h
#define __itkLevelOrderTreeIterator_h

#include <queue>
#include <itkTreeIteratorBase.h>

namespace itk{

template <class TTreeType>
class LevelOrderTreeIterator : public TreeIteratorBase<TTreeType> 
{
public:

  typedef TreeIteratorBase<TTreeType> Superclass; 
  typedef typename Superclass::Self Self;
  typedef TTreeType TreeType;
  typedef typename TTreeType::ValueType ValueType;
  typedef typename Superclass::TreeNodeType TreeNodeType;

  int GetType() const ;

  int GetStartLevel() const;
  int GetEndLevel() const;
  int GetLevel() const;

  LevelOrderTreeIterator( TreeType* tree, int endLevel = INT_MAX, const TreeNodeType* start = NULL);
  LevelOrderTreeIterator( TreeType* tree, int startLevel, int endLevel, const TreeNodeType* start = NULL);
  TreeIteratorBase<TTreeType>* Clone();

  Self& operator=(Superclass& iterator) {
  
    Superclass::operator=(iterator);
    LevelOrderTreeIterator<TTreeType>& it = static_cast<LevelOrderTreeIterator<TTreeType>&>(iterator);
    m_StartLevel = it.m_StartLevel;
    m_EndLevel = it.m_EndLevel;
    m_Queue = it.m_Queue;
    return *this;
  }


protected:
  const ValueType& Next();
  bool HasNext() const;

private:

  const TreeNodeType* FindNextNode() const;
  const TreeNodeType* FindNextNodeHelp() const ;
  int GetLevel( const TreeNodeType* node ) const;
  int m_StartLevel;
  int m_EndLevel;
  mutable std::queue<const TreeNodeType*> m_Queue;

};


/** Constructor with end level specification */
template <class TTreeType>
LevelOrderTreeIterator<TTreeType>
::LevelOrderTreeIterator( TTreeType* tree, int endLevel, const TreeNodeType* start)
 :TreeIteratorBase<TTreeType>(tree,start)
{
  m_StartLevel =  -1;
  m_EndLevel = endLevel;
  if ( start != NULL )
    {
    m_Queue.push( start );
    m_Position = const_cast<TreeNodeType*>(start);
    }
  else
    {
    if(tree->GetRoot())
      {
      m_Queue.push( dynamic_cast<const TreeNodeType*>(tree->GetRoot()) );
      m_Position = const_cast<TreeNodeType*>(dynamic_cast<const TreeNodeType*>(tree->GetRoot()));
      }
    }
  m_Begin = m_Position;
}



/** Constructor with end level specification */
template <class TTreeType>
LevelOrderTreeIterator<TTreeType>
::LevelOrderTreeIterator( TTreeType* tree, int startLevel, int endLevel, const TreeNodeType* start)
 :TreeIteratorBase<TTreeType>(tree,start)
{
  m_StartLevel = startLevel;
  m_EndLevel = endLevel;
  if ( start != NULL )
    {
    m_Queue.push( start );
    m_Position = const_cast<TreeNodeType*>(start);
    }
  else
    {
    if(tree->GetRoot())
      {
      m_Queue.push( dynamic_cast<const TreeNodeType*>(tree->GetRoot()) );
      m_Position = const_cast<TreeNodeType*>(dynamic_cast<const TreeNodeType*>(tree->GetRoot()));
      }
    }
  m_Begin = m_Position;
}

/**
 * Gibt den Iteratortyp zur�ck
 * @return Typ des Iterators
 */
template <class TTreeType>
int 
LevelOrderTreeIterator<TTreeType>::GetType( ) const 
{
  return TreeIteratorBase<TTreeType>::LEVELORDER;
}


/**
 * �berpr�ft, ob es ein weiteres Element gibt
 * @return true es gibt einen weiteren Knoten
 *         false es gibt keinen weiteren Knoten
 */
template <class TTreeType>
bool 
LevelOrderTreeIterator<TTreeType>::HasNext() const
{
 if(const_cast<TreeNodeType*>(FindNextNode()))
   {
   return true;
   }
  return false;
}


/**
 * Setzt den Iterator um eins weiter und gibt den Wert zur�ck
 * @return Wert des n�chsten Knotens
 */
template <class TTreeType>
const typename LevelOrderTreeIterator<TTreeType>::ValueType&
LevelOrderTreeIterator<TTreeType>::Next() 
{
  m_Position = const_cast<TreeNodeType* >(FindNextNode());
  return m_Position->Get();
}

/** Const Next */
/*
template <class TTreeType>
const typename LevelOrderTreeIterator<TTreeType>::TreeNodeType* 
LevelOrderTreeIterator<TTreeType>::Next() const
{
  m_Position = const_cast<TreeNodeType*>(FindNextNode());
  return m_Position;
}
*/

/** */
template <class TTreeType>
int LevelOrderTreeIterator<TTreeType>::GetStartLevel() const
{
  return m_StartLevel;
}


/** */
template <class TTreeType>
int 
LevelOrderTreeIterator<TTreeType>::GetEndLevel() const
{
  return m_EndLevel;
}

/** Find the next available node */
template <class TTreeType>
const typename LevelOrderTreeIterator<TTreeType>::TreeNodeType*
LevelOrderTreeIterator<TTreeType>::FindNextNode() const
{
  int level;
  const TreeNodeType* node;

  do{
    node = FindNextNodeHelp();
    if ( node == NULL )
      {
      return NULL;
      }
    level = GetLevel( node );
    if ( level > m_EndLevel )
      {
      return NULL;
      }
  } while ( level < m_StartLevel );

  return node;
}


/** Return the current level*/
template <class TTreeType>
int 
LevelOrderTreeIterator<TTreeType>::GetLevel() const
{
  if( m_Position == NULL )
    {
    return -1;
    }
  
  int level = 0;
  TreeNodeType* node = m_Position;
  while( node->HasParent() && node != m_Root ) 
    {
    node = dynamic_cast<TreeNodeType*>(node->GetParent());
    level++;
    }
  return level;
}



/** Return the level given a node */
template <class TTreeType>
int 
LevelOrderTreeIterator<TTreeType>::GetLevel( const TreeNodeType* node ) const
{
  if( node == NULL )
    {
    return -1;
    }
  int level = 0;

  while( node->HasParent() && node != m_Root )
    {
    node = dynamic_cast<const TreeNodeType*>(node->GetParent());
    level++;
    }
  return level;
}


/** Helper function to find the next node */
template <class TTreeType>
const typename LevelOrderTreeIterator<TTreeType>::TreeNodeType* 
LevelOrderTreeIterator<TTreeType>::FindNextNodeHelp() const
{
  if( m_Queue.empty() )
    {
    return NULL;
    }
 
  const TreeNodeType *currentNode = m_Queue.front();
  m_Queue.pop();

  if ( currentNode == NULL)
    {
    return NULL;
    }

  int size = currentNode->CountChildren();

  for ( int i=0; i < size; i++ ) 
    {
    TreeNodeType *child = dynamic_cast<TreeNodeType*>(currentNode->GetChild( i ));
    if ( child != NULL )
      {
      m_Queue.push(child);
      }
    }

  // If the current node is the root we try again
  if(currentNode == m_Root)
    {
    currentNode = const_cast<TreeNodeType*>(FindNextNodeHelp());
    }
  return currentNode;
}

/** */
template <class TTreeType>
TreeIteratorBase<TTreeType>* LevelOrderTreeIterator<TTreeType>::Clone() 
{
  LevelOrderTreeIterator<TTreeType>* clone = new LevelOrderTreeIterator<TTreeType>( const_cast<TTreeType*>(m_Tree), m_StartLevel, m_EndLevel );
  *clone = *this;
  return clone;
}



} // end namespace itk

#endif
