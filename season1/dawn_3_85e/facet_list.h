#if !defined FACET_LIST_H
#define FACET_LIST_H

	//---------- clear edge list of facetItem
inline void List<Facet>::clearItem()
{
	if( cur == NULL) {
		return ;
	} else {
		(cur->item).clear();
	}

} // List<Type>::List()

#endif
