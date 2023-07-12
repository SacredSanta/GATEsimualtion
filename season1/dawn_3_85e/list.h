//////////////////////////////////
/////  list.h  (1995/06/04) /////
//////////////////////////////////
////////////////////////////////////////////////////////////////////
///// Class for Multi-purpose linear linked list:
/////
/////   For example, a list of type "T" items is instanciated as:
/////     List<T> list ;
/////
/////   There are three ways of using an instance of this class:
/////   (1) as bilinear list (using addItem(), getItem() etc)
/////   (2) as stack ( using push() and pop etc )
/////   (3) as cyclic list ( using toCounterClockWise(), toClockWise() etc)
/////
///// << Attention >>
///// (1) T::operator = () should be defined in order to
/////     add items to the list, though it is not necessary
/////     in many cases.
///// (2) Type "T" may be a class.
/////      If so, its "constructor with no argument" must be defined.
//////////////////////////////////////////////////////////////////////
//            << Structure of linked list >>
//
//                               (mark, mark2)
//                                ||                
//  ..........  head      up <--- cur ---> down       tail ..........
//  |            ||               ||                   ||            |
//  |  NULL --- Node --- Node --- Node --- ...... --- Node --- NULL  |
//  |            |        |        |                   |             |
//  |          item     item     item                item            |
//  |                                                                |
//  |........>>>...........................................<<<........
//       counter clock-wise                             clock-wise
//       direction                                      direction
/////////////////////////////////////////////////////////////////////


#if !defined LIST_H
#define LIST_H

#include <iostream.h>	
#include <stdlib.h>	
#include <string.h>

////////////////////////
/////  class Node  /////
////////////////////////
//----- Node of list
template <class Type> class Node {
 public:
	Type item ;		// item
	Node<Type>* down ;	// address of neighboring node (downward)
	Node<Type>* up ;	// address of neighboring node (upward)
} ; // Node


///////////////////////////////
/////  class List<Type>  /////
//////////////////////////////
//----- class List
template <class Type> class List {
 private:
		//----- constants
	enum { ERROR = 0, NORMAL = 1 };
	enum { NAME_LENGTH = 16 } ;

		//----- name of list
	char name[ NAME_LENGTH ] ;

		//----- head, tail and current nodes etc.
		//.....  head: node added most recently
		//.....  tail: node added at first
		//.....  cur:  current node
		//.....  down: direction to tail
		//.....  up:   direction to head
		//.....
		//.....  head->up = NULL, and  tail->down == NULL
		//
        	//          head      up <--- cur ---> down       tail
                //	   ||               ||                   ||
	        // NULL --- Node --- Node --- Node --- ...... --- Node --- NULL
        	//           |        |        |                   |
                //	  item     item     item                item
		//
	Node<Type>*	head ;	// head of list
	Node<Type>*	tail ;	// tail of list
	Node<Type>*	cur ;	// current node
	Node<Type>*	mark ;	// marked node No 1
	Node<Type>*	mark2 ;	// marked node No 2.

		//----- number of items in list
	int		nItem ; // number of stored items	

 public:
		//----- Constructor
		//.....  Initialize head and current node of list to NULL.
		//.....  Set name of list ( default name is "no_name")
	List( char* name_ = "no_name" ) ;

		//----- Destructor
		//..... call this->clear()
	~List() { this->clear() ; }
	
		//----- set and get name of list
	void setName( char* name_ = "no_name" ){ strcpy( name, name_ ); }

		//----- get name of list
	const char* getName() const { return name; }

		//----- Add an item to list and make it a new head.
		//.....  Return: NORMAL = 1  (normal)
		//.....          ERROOR = 0  (error)
	int	addItem( const Type& item );	

		//----- Insert an  item
		//.....  A new node with the given item
		//.....  is inserted upon the current node.
		//.....  Inserted new node becomes a new current node.
		//.....  Return: NORMAL = 1  (normal)
		//.....          ERROOR = 0  (error)
	int	insertItem( const Type& item );	

		//----- operator += ( added_list )
	void operator += ( List<Type>& added );

		//----- operator = ( added_list )
		//.....  All items are copied from rhs to lhs
		//....   "head" and "tail" are properly reset.
		//.....  "name" is copied.
		//.....  "cur" is reset  to "head".
		//.....  "mark"  is reset to NULL.
		//.....  "mark2" is reset to NULL.
	void operator = ( const List<Type>& added );

		//----- overwrite current item with given one
	int overwriteItem( const Type& item ) ;

		//----- overwrite n-th item with given one
		//.....  (The "tail" is the 0-th node.)
		//.....  Return NORMAL = 1: if successfully overwrited.
		//.....                     Current node moves to n-th one.
		//.....         ERROR  = 0: if n >= nItem or n < 0
		//.....                     Current node does not move.
	int overwriteItem( int n , const Type& item ) ;

		//----- Delete an item of current node
		//.....  Return: NORMAL = 1  if deleted.
		//.....          ERROR  = 0  if not deleted ( cur = NULL or nItem = 1).
		//.....  In case of NORMAL, cur moves to cur->down.
	int	deleteItem() ;


		//----- devide list
		//.....  Move a part of list and add it to output list
		//.....  Note:
		//.....  	Nodes ( >=a && <= b ) are added to output 
		//.....  	list. Then,
		//.....  	if mode == 0 these nodes are deleted from *this, 
		//.....  	if mode == 1 (default), these nodes 
		//.....		 "except edges a and b" are removed,
		//.....         if mode == else, no items are deleted
		//.....  Argument:
		//.....  	a: beginning node label of the moved part
		//.....  	b: ending    node label of the moved part
		//.....		output_list: output list
		//.....  Return: ERROR or NORMAL
		//.....		ERROR is returned,
		//.....		if a < 0, a >= nItem,  b < 0, b >= nItem, 
		//.....         or a == b.
	int	devideCyclic( int a, int b, List<Type>* output_list_p, int mode = 1 );


		//----- Set current node to the head of list.
	void	toHead()    { cur = head ; } ;

		//----- Set current node to the tail of list.
	void	toTail()    { cur = tail ; } ;

		//----- Increment current node to downward direction by 1 step
		//.....  Return: NORMAL = 1  (normal)
		//.....          ERROOR = 0  (if cur = NULL)
	int	downward();		

		//----- Increment current node to downward direction by n steps
		//.....  Return: number of incremented steps
	int	downward( int n );	

		//----- Increment current node to upward direction by 1 step
		//.....  Return: NORMAL = 1  (normal)
		//.....          ERROOR = 0  (if cur = NULL)
	int	upward();		

		//----- Increment current node to upward direction by n steps
		//.....  Return: number of incremented steps
	int	upward( int n );	

		//----- Same as upward() except that
		//.....  next to head is tail.
		//.....  Return: NORMAL = 1  (normal)
		//.....          ERROR  = 0  (cur == NULL or nItem == 0 )
		//.....          If ERROR, cuuren node does not move.
	int	toCounterClockWise();

		//----- Same as downward() except that
		//.....  next to tail is head.
		//.....  Return: NORMAL = 1  (normal)
		//.....          ERROR  = 0  (cur == NULL or nItem == 0 )
		//.....          If ERROR, cuuren node does not move.
	int	toClockWise();

		//----- Move current node to n-th one from tail
		//.....  (The "tail" is the 0-th node.)
		//.....  Return NORMAL = 1: if successfully moved.
		//.....         ERROR  = 0: if n >= nItem or n < 0
		//.....                     Current node does not move.
	int	goTo( int n );

		//----- Get current item
		//.....  The obtained item is returned to the argument.
		//.....  Return: NORMAL = 1  (normal)
		//.....          ERROR = 0   (error)
	int	getItem( Type& item );

		//----- Get address of current item
		//.....  The obtained item is returned to the argument.
		//.....  Return: NORMAL = 1  (normal)
		//.....          ERROR = 0   (error)
	Type*	getItemAddress() ;

		//----- Get upward next item
		//.....  An obtained item is returned to the argument.
		//.....  Return: NORMAL = 1  (normal)
		//.....          ERROR  = 0  (cur == NULL or nItem <= 1 )
		//.....  If cur == head, tail->item is returned.
	int	getUpwardItemCyclic( Type& item );

		//----- Get downward next item
		//.....  An obtained item is returned to the argument.
		//.....  Return: NORMAL = 1  (normal)
		//.....          ERROR  = 0  (cur == NULL or nItem <= 1 )
		//.....  If cur == tail, head->item is returned.
	int	getDownwardItemCyclic( Type& item );

		//----- Return n-th item counting from tail
		//.....  (The "tail" is the 0-th node.)
		//.....  Current node moves at the same time.
	Type& operator [] ( int n ) ;

		//----- Get position of current node counting from tail
		//.....  Return: 0, 1, 2, ..... , nItem -1
		//.....  If cur == tail, 0 is returned.
		//.....  If cur == NULL, -1 is returned.
		//.....  Current node is not modified after calling this function
	int whereIsCurrentNode() ;

		//----- Check if current node is NULL
		//.....  Return 1: if cur == NULL
		//.....         0: if cur != NULL
	int isCurrentNodeNull() { return ( cur == NULL ) ; }

		//----- Get number of stored items
	int 	getNItem() const { return nItem ; }

		//----- delete all items
		//..... All nodes are deleted.
		//..... head, tail, cur, mark, and mark2 are reset to NULL.
		//..... Note that member "name" is not cleared.
	void clear();

		//----- set mark to current node
		//.....  Return NORMAL, if mark successfully
		//.....                 set to currrent node
		//.....         ERROR,  if current node is NULL
		//.....  If ERROR, old mark remains.
	int setMark ()  ;
	int setMark2()  ; // same function for  mark2

		//----- see if mark is already set
		//.....  Return 1 if mark is set
		//.....         0 if mark is not set ( i.e. mark = NULL )
	int isMarkSet()  { return ( mark  != NULL ) ; }
	int isMarkSet2() { return ( mark2 != NULL ) ; } 
			// same function for  mark2

		//----- clera mark
	void clearMark()  { mark  = NULL ; }
	void clearMark2() { mark2 = NULL ; } // same function for  mark2
		

		//----- move current node to marked node
		//.....  Return NORMAL if successfully moved
		//.....         ERROR  if mark = NULL.
		//.....         If ERROR, current node does not move.
	int  toMark()  ;
	int  toMark2()  ;	// same function for  mark2

		//----- Get position of marded node counting from tail
		//.....  Return: 0, 1, 2, ..... , nItem -1
		//.....  If mark == NULL, -1 is returned.
		//.....  Current node is not modified after calling this function
	int  whereIsMark()  ;
	int  whereIsMark2()  ;

		//----- check if current node is the marked node
	int isCurrentNodeMarked() { return ( cur == mark  && cur != NULL ) ; }
	int isCurrentNodeMarked2(){ return ( cur == mark2 && cur != NULL ) ; }

		//---- overwrite marked item
	int overwriteMarkedItem  ( const Type& item ) ;
	int overwriteMarked2Item ( const Type& item ) ; 

		//----- convert list to array
		//.....  Argument: itemArray --> array of "Type" type
		//.....            nArray    --> dimension of itemArray
		//.....  Return:   number of substituted items to Array.
		//.....  Note: If nArray > nItem, items are substitued only to
		//.....        itemArray[ 0 <= i < nItem ]'s, and
		//.....        itmeArray[ i>= nItem]'s are not modified.
		//.....        If nArray < nItem, only nArray items from tail
		//.....        are substituted to itemArray[].
	int toArray( Type* itemArray, int nArray ) ;

		//----- add elements of array to list
		//.....  Argument: itemArray --> array of "Type" type
		//.....            nArray    --> dimension of itemArray
		//.....  Return:   number of added items
	int addArray        ( Type* itemArray, int nArray ) ;
	int addArrayReverse ( Type* itemArray, int nArray ) ;

		//----- For use of STACK: push
		//.....  Added new node becomes a new current node.
		//.....  Return: NORMAL = 1  (normal)
		//.....          ERROOR = 0  (error)
		//.....  Added new node becomes a new current node.
	int	push( const Type& item ) { return addItem( item ) ; }

		//----- For use of STACK: pop
		//.....  Get head->item and then delete it.
		//.....  The obtained item is returned to the argument.
		//.....  Return: NORMAL = 1  (normal)
		//.....          ERROR = 0   (error)
		//.....  In case of NORMAL, cur moves to cur->down.
	int	pop( Type& item ) ;

		//----- clear current item
		//.....  Implimentation of this function is not included in
		//.....  this header file, since it depends of type of item.
		//.....  Implimentation should be given, for example, as:
		//.....   List<int>::clearItem(){ cur->item = 0 ;}
	void clearItem();

} ; // class List



////////////////////////////////////////////////////////
/////  Inline Member functions of class "List"  ///////
////////////////////////////////////////////////////////

	//----- Constructor: List()
template <class Type>
inline List<Type>::List( char* name_ )
{
	cur = head = tail = NULL ;
	clearMark();	clearMark2();
	nItem = 0 ;
	setName( name_ ) ;

} // List<Type>::List()


	//----- Destructor: ~List()
	//.....  Defined in specification part


	//----- setName()
	//.....  Defined in specification part


	//----- getName()
	//.....  Defined in specification part


	//----- addItem
template <class Type>
inline int List<Type>::addItem( const Type& item )
{
		//----- local
	int		status ;
	Node<Type>*	old_head = head ; // store old head

		//----- allocate dynamical memory for added node
	head = new Node<Type> ;		
	if( !head ){
			//----- failed to allocate dynamical memory
		status = ERROR ;
//		return ERROR ;
	} else {
			//----- increment number of stored items
		nItem++ ;				
	
			//----- set tail of list
		if( nItem == 1 ) { tail = head ; }
	
			//----- set current node to added node
		cur = head ;
	
			//----- add an item
		head->item = item ;	// add an item
	
			//----- make links
			//.....          2          1
			//.....  NULL <----- head -----> old_head
			//.....                   <-----
			//.....                     3
		head->down = old_head ; // make link 1
		head->up   = NULL ;	// make link 2
		if( old_head != NULL ) {
			old_head->up = head ; // make link 3
		}
			//----- set status	
		status = NORMAL ;
	} // if--else

		//----- return status
	return status ;		

} // List<Type>::addItem()


	//----- insertItem
template <class Type>
inline  int List<Type>::insertItem( const Type& item )
{
		//----- local
	int	status  ;

		//----- if cuurent node is NULL ( but items exist),
		//..... give up insertion
	if( nItem != 0 && cur == NULL ) { 
			//----- should not happen
		cerr << "WARNING: in List<Type>::insertItem(), 1\n";
		status =  ERROR ; 
	}
		//----- if currrent node is head, just call addItem()
	else if( cur == head || nItem == 0 ) {
		this->addItem( item ); 
		status = NORMAL ;
	} else {
			//----- Below, at least one item should already exist.
			//..... The old_cur is not head.

			//----- store old current node
		Node<Type>* old_cur   = cur  ;
	
			//----- set the given item to a new node
		cur = new Node<Type> ;	
		cur->item = item ;	nItem++ ;	

			//----- Make links
			//...... In the following, nItem >= 2
			//.....  (including the added item), 
			//.....  and old_cur is neighter NULL nor head.
		if( old_cur == NULL ) {
				//----- should not happen
			cerr << "ERROR in List<Type>::insertItem(), 2\n";
		} else if( old_cur == head ) {
				//----- should not happen
			cerr << "ERROR in List<Type>::insertItem(), 3\n";

		} else { //----- in case that old cur is neither head nor tail
			cur->down	= old_cur ;
			cur->up		= old_cur->up  ;
			(cur->down)->up	= cur ;
			if( (cur->up) != NULL ) { (cur->up)->down = cur ; }
		}
			//----- set status
		status = NORMAL ;
	} // if--else

		//----- return status
	return status ;			
} // List<Type>::insertItem()


	//----- operator += ()
template <class Type>
inline void List<Type>::operator += ( List<Type>& added )
{
		//----- item of added lists
	Type added_item;
	
		//-----  add items of added list beginning from tail of it
	added.toTail() ;
	while( added.getItem(added_item) ) {
		this->addItem( added_item );
		added.upward() ;
	}

} // List<Type>::operator += ()


	//----- operator = ()
template <class Type>
inline  void List<Type>::operator = ( const List<Type>& rhs_list )
{
		//----- local variables
	Node<Type>* rhs_cur ;
	int	i, rhs_nItem, larger_nItem ;
	int	status, status_rhs ;
	char    name_tmp[NAME_LENGTH] ;

		//----- number of item in rhs list
	rhs_nItem	= rhs_list.getNItem();

		//----- max( nItem, rhs_nItem)
	larger_nItem	= ( nItem > rhs_nItem ? nItem : rhs_nItem );

		//-----  copy lists
	rhs_cur = rhs_list.tail ;
	this->toTail();
	for ( i = 0 ; i < larger_nItem ; i++ ) {
		Type	item, dummy ;

		status     = this->getItem( dummy ) ;
		status_rhs = ( i >= 0 && i < rhs_nItem ) ;
		if( status_rhs ) { item = rhs_cur->item ; }

		if        ( status == 1 && status_rhs == 1 ) {
		 	this->overwriteItem( item ) ;
			rhs_cur = rhs_cur->up ; // update rhs
			this->upward();         // update lhs
		} else if ( status == 1 && status_rhs == 0 ) {
			this->deleteItem();
			if( cur == NULL ) {
				this->toTail(); // update lhs
			} else {
				this->upward(); // update lhs
			}
		} else if ( status == 0 && status_rhs == 1 ) {
			this->addItem( item ) ;
			rhs_cur = rhs_cur->up ; // update rhs
			this->upward();         // update rhs (dummy)
		} else if ( status == 0 && status_rhs == 0 ) {
//			return ;
			break ;
		} else {
			cerr << "ERROR ( List<Type>::operator=()) 1\n";
//			exit(1);
			break ;
		}

	} // for(i)

		//----- error checking
	if( nItem != rhs_nItem ) {
		cerr << "ERROR ( List<Type>::operator=()) 2\n";
//		exit(1);
	}

		//----- other members
	strcpy( name_tmp, rhs_list.getName() ) ;
	this->setName( name_tmp ) ;		// "name" is copied
	this->clearMark();			// "mark" is cleared (NULL)
	this->clearMark2();			// "mark2" is cleared (NULL)
	this->toHead() ;			// "cur" is reset to "head"

} // List<Type>::operator = ()


	//----- overwriteItem()
template <class Type>
inline  int List<Type>::overwriteItem( const Type& item_ )
{
		//----- local
	int	status ;

		//----- current node is NULL or no item in list
	if ( cur == NULL || nItem == 0 ) {
		status =  ERROR ;
	} else {
			//----- overwrite item of cuurent node
		cur->item = item_ ;
		status =  NORMAL ;
	}
	
		//----- normal return
	return status ;

} // List<Type>::overwriteItem()


	//----- overwriteItem()
template <class Type>
inline  int List<Type>::overwriteItem( int n , const Type& item )
{
		//----- local variables
	int ret_status;
	int status;

		//----- move "cur" to n-th node from tail
		//.....  If successfully moved,
		//.....  NORMAL = 1 is returned to status.
		//.....  If movement failed,
		//.....  ERROR = 0 is returned to status.
	status = this->goTo( n ) ;

		//----- overwrite
	if( status ) {
			//----- if cur is successfully moved,
			//.....  overwrite an item there
		this->overwriteItem( item );
		ret_status = NORMAL ;
//		return NORMAL ;
	} else {
			//----- if cur failed to move,
			//.....  do not overwrite
		ret_status = ERROR ;
//		return ERROR;
	}

		//----- return status of this function
	return ret_status ;

} //List<Type>::overwriteItem()


	//----- deleteItem()
template <class Type>
inline  int List<Type>::deleteItem()
{
		//----- local variables
	int status ;
	Node<Type>* cur_old ;	// backup of current node

		//----- ERROR if current node does not have an item
		//...... or no item exist in list
	if ( cur == NULL || nItem == 0 ) {
		status = ERROR ;
//		return ERROR ;
	} else {
			//----- Below cur != NULL, nItem >= 1, 
			//..... head != NULL, and tail != NULL
	
			//----- reset mark if it is on current node
		if( mark  == cur && mark  != NULL ) { clearMark()  ; }
		if( mark2 == cur && mark2 != NULL ) { clearMark2() ; }
	
			//----- make backup of current node
		cur_old  = cur ;
	
			//----- remake links
		if( cur == head ) {
			cur  = cur->down ;
			head = cur ;
			if ( head != NULL ) {
				head->up = NULL ;
				delete cur_old ;  nItem-- ;
			} else {
					//----- In case that head = NULL:
					//.....  This case occurs if
					//.....  head = tail i.e. nItem = 1 originally.
					//.....  At the end of this function,
					//.....  head = tail = cur = NULL, and nItem = 0
				tail = NULL ;
				delete cur_old ; 
				nItem = 0 ;
				// nItem-- ;
			}
	
		} else if( cur == tail ){
				//----- Here, cur != head && cur == tail.
				//.....  So, tail != head.
				//.....  It means, at least 2 items exist in the list.
				//.....  So, tail->up is not NULL.
			tail = tail->up ;
			tail->down = NULL;
			cur = NULL ;
			delete cur_old ;  nItem-- ;
	
		} else {	 //----- In case that cur is neither head or tail.
				 //.....  head --- .... --- cur ---... --- tail.
			(cur->up)->down   = cur->down ;
			(cur->down)->up   = cur->up   ;
			cur = cur->down ;
			delete cur_old ;  nItem-- ;
		} // if-else_if-else
		
		status = NORMAL;

	} // if--else

		//----- return (successfully deleted)
	return status ;

} // List<Type>::deleteItem()


	//----- List<Type>::devideCyclic()
template <class Type>
inline  int List<Type>::devideCyclic( int a, int b, List<Type>* output_list_p, int mode )
{
		//----- local variables
	int		status ;
	Node<Type>*	start_node ;
	Node<Type>*	end_node ;
	int		end_flag ;

		//----- error checking
	if( a < 0 || a >= nItem || b < 0 || b >= nItem || a == b ) {
		cerr << "ERROR (List<Type>::devide(int a, int b, List<T>* output_list ) )\n";
		cerr << "  Irregal paramaters in a list of " << nItem << " items: ";
		cerr << "a = " << a << " , b = " << b << endl;
		status = ERROR ;
//		return ERROR ;
	} else { //****
	
			//----- store ending node 
		goTo( b ) ;	end_node   = cur ;
		
			//----- move cur to starting node
			//.....  and store it
		goTo( a ) ;	start_node = cur ;
	
			//----- move a part of *this to output list
		end_flag = 0 ;	// initialize end_flag
		do {
				//----- Is this executionthe the last one?
			if( cur == end_node ) { end_flag = 1 ; 	}
	
				//----- copy item of current node to output list
			output_list_p->addItem( cur->item ) ;
	
				//----- delete current item from *this
				//..... mode:
				//.....  0: any item deleted
				//.....  1: items of starting and ending nodes 
				//.....     are not deleted
				//.....  else: no items are deleted.
			if( mode == 0 ) {
				deleteItem() ;
			} else if( mode == 1 && cur != start_node && cur != end_node ) {
				deleteItem() ;
			}
				//----- updation
			if( cur == NULL ) { 
				toTail() ;
			} else {
				toCounterClockWise(); 
			}
	
		} while( !end_flag ) ;
		
		status = NORMAL ;
	} // if--else

		//----- normal return
	return status ;

} // List<Type>::devideCyclic()

	//----- toHead()
	//.....  Defined in specification part

	//----- toTail()
	//.....  Defined in specification part


	//----- downward()
template <class Type>
inline  int List<Type>::downward()
{
	int status ;
	if( cur == NULL ) {
		status = ERROR ; // do nothing
//		return ERROR ;  
	} else {
		cur = cur->down ; // move dawnward
		status = NORMAL ;
//		return NORMAL ;
	}
	return status ;
} // List<Type>::downward()


	//----- downward(int)
template <class Type>
inline  int List<Type>::downward( int n )
{
		//----- local variables
	int i ;
	int state  ;
	int how_many_steps = 0 ;

		//----- call downward(void) n times
	for (i = 0; i < n ; i++ ) {
		state = this->downward();
		if(state)  {
			how_many_steps++ ;
		} else {
			break ;
		}
	}

		//----- return number of steps really done
	return how_many_steps ;

} // List<Type>::downward(int)


	//----- upward(void)
template <class Type>
inline  int List<Type>::upward()
{
	int status ;
	if( cur == NULL ) {
		status = ERROR ; // do nothing
//		return ERROR ;  // current node is NULL
	} else {
		cur = cur->up ; // move upward
		status = NORMAL ;
//		return NORMAL ;
	}
	return status ;
} // List<Type>::upward(void)


	//----- upward(int)
template <class Type>
inline  int List<Type>::upward( int n )
{
		//----- local variables
	int i ;
	int state  ;
	int how_many_steps = 0 ;

		//----- call upward(void) n times
	for (i = 0; i < n ; i++ ) {
		state = this->upward();
		if(state) {
			how_many_steps++ ;
		} else {
			break ;
		}
	}

		//----- return number of steps really done
	return how_many_steps ;

} // List<Type>::upward(int)


	//----- toCounterClockWise()
template <class Type>
inline  int List<Type>::toCounterClockWise()
{
		//----- return value
	int state ;

		//----- move cur to counter-clock-wise (upward) direction
	if ( cur == NULL || nItem == 0 ) {
			//----- error: no next item
		state = ERROR ;
	} else if ( cur == head ) {
			//----- if current node is head,
			//..... move current node to tail.
		state = NORMAL ;
		toTail() ;
	} else { // Since cur != NULL and cur != head  here,
		 // cur->up is not NULL.
		 // If nItem is 1, this part is never executed.
			//----- move cur to cur->up
		state = NORMAL ;
		upward();
	}

		//----- return
	return state ;

} // List<Type>::toCounterClockWise()


	//----- toClockWise()
template <class Type>
inline  int List<Type>::toClockWise()
{
		//----- return value
	int state ;

		//----- move cur to clock-wise (downward) direction
	if ( cur == NULL || nItem == 0 ) {
			//----- error : no next item
		state = ERROR ;
	} else if ( cur == tail ) {
			//----- If current node is tail,
			//..... move current node to head.
		state = NORMAL ;
		toHead();
	} else {// Since cur != NULL and cur != tail  here,
		// cur->down is not NULL.
		// If nItem is 1, this part is never executed.
			//----- move cur to cur->down
		state = NORMAL ;
		downward();
	}

		//----- return
	return state ;

} // List<Type>::toClockWise()


	//----- goTo( int n )
template <class Type>
inline  int List<Type>::goTo( int n )
{
		//----- local
	int status ;

		//----- check if the n-th node exists
	if( n >= nItem || n < 0 ) { 
		status = ERROR ; // do nothing
//		return ERROR ; 
	} else {
			//----- move to n-th node
		this->toTail();
		this->upward(n);
		status = NORMAL ;
	}
		//----- return status
	return status ;
//	return NORMAL ;
} // List<Type>::goTo()

	//----- getItem()
template <class Type>
inline  int List<Type>::getItem( Type& item )
{
		//----- return value
	int state ;

		//----- get an item of current node
	if ( cur == NULL ) {
		state = ERROR ;
	} else {
		state = NORMAL ;
		item = cur->item ;
	}

		//----- return
	return state ;
} // List<Type>::getItem()


	//----- getItem()
template <class Type>
inline Type* List<Type>::getItemAddress()
{
	Type* retValue ;

	if( NULL == cur ) { 
		retValue  = NULL ;
//		return NULL ;
	} else {
		retValue = &(cur->item);
//		return &(cur->item);
	}
	return retValue ;
} // List<Type>::getItemAddress()


	//----- getUpwardItemCyclic()
template <class Type>
inline  int List<Type>::getUpwardItemCyclic( Type& item )
{
		//----- return value
	int state ;

		//----- get an item of upward next node
	if ( cur == NULL || nItem <= 1 ) {
			//----- error: no next item
		state = ERROR ; // do nothing
	} else if ( cur == head ) {
			//----- if current node is head, return item of tail
		state = NORMAL ;
		item = tail->item ;
	} else { //----- cur->up is not NULL
		 //...... Note that nItem >= 2  and that cur != NULL here.
		 //.....  Since cur is not head, cur->up is not NULL.

			//----- return item of upward next item
		state = NORMAL ;
		item = (cur->up)->item ;
	}

		//----- return
	return state ;

} // List<Type>::getUpwardItemCyclic()


	//----- getDownwardItemCyclic()
template <class Type>
inline  int List<Type>::getDownwardItemCyclic( Type& item )
{
		//----- return value
	int state ;

		//----- get an item of downward next node
	if ( cur == NULL || nItem <= 1 ) {
			//----- error : no next item
		state = ERROR ; // do nothing
	} else if ( cur == tail ) {
			//----- If current node is tail, return item of head.
		state = NORMAL ;
		item = head->item ;
	} else { //----- cur->down is not NULL
		 //...... Note that nItem >= 2  and that cur != NULL here.
		 //.....  Since cur != tail, cur->down is not NULL.

			//----- return item of downward next item
		state = NORMAL ;
		item = (cur->down)->item ;
	}

		//----- return
	return state ;

} // List<Type>::getDownwardItemCyclic()


	//----- operator []
template <class Type>
inline Type& List<Type>::operator [] ( int n )
{
		//----- check if given "n" is in the right range
		//..... i.e. 0, 1, 2, ... nItem -1
	if( n >= nItem || n < 0 || nItem == 0 ) {
		cerr << "ERROR: (List<Type>::operator[]):\n";
		cerr <<	"  Irregal item label " << n << "\n";
		cerr <<	"  for nItem = " << nItem << ".\n" ;
		cerr << "  Label n is set to 0" << endl;
		n = 0 ;
//		exit(1) ;
	}

		//----- move cur to n-th node from tail
	this->goTo( n );

		//----- return item
	return cur->item ;

} // List<Type>::operator [] ( int n )


	//----- List<Type>::whereIsCurrentNode()
template <class Type>
inline  int List<Type>::whereIsCurrentNode()
{
	Node<Type>* tmp = cur ;		// copy current node
	int n = -1 ;			// counter

		//----- count how many steps there are until tail.
	while( tmp != NULL ) {	n++ ; tmp = tmp->down ;}

		//----- return steps
	return n ;

} // List<Type>::whereIsCurrentNode()


	//----- isCurrentNodeNull()
	//.....  Defined in specification part


	//----- getNItem
	//.....  Defined in specification part


	//----- clear()
template <class Type>
inline  void List<Type>::clear()
{
		//----- delete items
	cur = head ;
	while( cur != NULL )
	{
		Node<Type>* tmp = cur ;
		cur = cur->down ;
		delete tmp ;	// destructor called
	}

		//----- reset head and current node
	cur = head  = tail = NULL ;

		//----- clear marks
	this->clearMark();			// "mark" is cleared (NULL)
	this->clearMark2();			// "mark2" is cleared (NULL)

		//----- reset number of stored items
	nItem = 0 ;

} // List<Type>::clear()


	//----- toMark()
template <class Type>
inline  int List<Type>::toMark()
{
		//----- local variables
	int status ;

	if( mark == NULL ) {
			//----- if no mark,
			//.....  current node does not move.
		status = ERROR ;
	} else {
		status = NORMAL ;
			//----- jump cur to mark
		cur = mark ;
	}

		//-----return status
	return status ;

} // List<Type>::toMark()



	//----- toMark2()
template <class Type>
inline  int List<Type>::toMark2()
{
		//----- local variables
	int status ;

	if( mark2 == NULL ) {
			//----- if no mark2,
			//.....  current node does not move.
		status = ERROR ;
	} else {
		status = NORMAL ;
			//----- jump cur to mark2
		cur = mark2 ;
	}

		//-----return status
	return status ;

} // List<Type>::toMark2()


	//----- List<Type>::whereIsMark()
template <class Type>
inline  int List<Type>::whereIsMark()
{
	Node<Type>*	tmp = mark ;	// copy marked node
	int		n   = -1 ;	// counter

		//----- count how many steps there are until tail.
	while( tmp != NULL ) {	n++ ; tmp = tmp->down ;}

		//----- return steps
	return n ;

} // List<Type>::whereIsMark()


	//----- List<Type>::whereIsMark2()
template <class Type>
inline  int List<Type>::whereIsMark2()
{
	Node<Type>*	tmp = mark2 ;	// copy marked node
	int		n   = -1 ;	// counter

		//----- count how many steps there are until tail.
	while( tmp != NULL ) {	n++ ; tmp = tmp->down ;}

		//----- return steps
	return n ;

} // List<Type>::whereIsMark2()


	//----- setMark()
template <class Type>
inline  int List<Type>::setMark()
{
		//----- local variables
	int	status ;
	
		//----- set mark to non-NULL cuurrent node
	if( cur != NULL ) {
		status = NORMAL ;
		mark = cur ;
	} else {
		status = ERROR ;		
	}

		//----- RETURN
	return status ;

} // List<Type>::setMark()




	//----- setMark2()
template <class Type>
inline  int List<Type>::setMark2()
{
		//----- local variables
	int	status ;
	
		//----- set mark2 to non-NULL cuurrent node
	if( cur != NULL ) {
		status = NORMAL ;
		mark2 = cur ;
	} else {
		status = ERROR ;		
	}

		//----- RETURN
	return status ;

} // List<Type>::setMark2()




	//----- getItem()
template <class Type>
inline  int List<Type>::overwriteMarkedItem( const Type& item )
{
		//----- local
	int	status ;

		//----- current node is NULL or no item in list
	if ( mark == NULL || nItem == 0 ) {
		status =  ERROR ;
	} else {
			//----- overwrite item of cuurent node
		mark->item = item ;
		status =  NORMAL ;
	}
	
		//----- normal return
	return status ;


} 

	//----- getItem()
template <class Type>
inline  int List<Type>::overwriteMarked2Item( const Type& item )
{
		//----- local
	int	status ;

		//----- current node is NULL or no item in list
	if ( mark2 == NULL || nItem == 0 ) {
		status =  ERROR ;
	} else {
			//----- overwrite item of cuurent node
		mark2->item = item ;
		status =  NORMAL ;
	}
	
		//----- normal return
	return status ;

} 


	//----- toArray()
template <class Type>
inline  int List<Type>::toArray( Type* itemArray, int nArray )
{
		//----- local variables
	int  number ;	// number of substituted items to itemArray[]
	Type item   ;

		//----- copy items to array from tail
	toTail() ; number = 0 ;
	while( getItem( item ) && number < nArray && number < nItem ) {
		itemArray[number++] = item ;	// substitution
		upward();
	}

		//----- return size of obtained array
	return number ;

}// List<Type>::toArray()


	//----- addArray()
template <class Type>
inline  int List<Type>::addArray( Type* itemArray, int nArray )
{
		//----- local variables
	int	i ;
	int	number = 0 ;

		//----- add items taken from array
	for( i = 0 ; i < nArray ; i++ ) {
		addItem( itemArray[i] ) ;
		number++ ;
	}

		//----- return number of added items
	return number ;

}// List<Type>::addArray()


	//----- addArrayReverse()
template <class Type>
inline  int List<Type>::addArrayReverse( Type* itemArray, int nArray )
{
		//----- local variables
	int	i ;
	int	number = 0 ;

		//----- add items taken from array
	for( i = (nArray - 1) ; i >= 0 ; i-- ) {
		addItem( itemArray[i] ) ;
		number++ ;
	}

		//----- return number of added items
	return number ;

}// List<Type>::addArray()


	//----- pop()
template <class Type>
inline  int List<Type>::pop( Type& item )
{
		//----- local variables
	int	status ;

		//----- get head->item
	toHead() ;
	status = getItem( item ) ;

		//----- delete head->item if it exists
		//.....  and make cur move to cur->down
	if( status ) { deleteItem(); }

		//----- RETURN
	return status ;

} // List<Type>::pop()



//////////////
#endif
////////////// end of list.h
