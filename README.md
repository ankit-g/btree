**B-Tree Organization**
----------------------

**A B-Tree is a balanced tree. Each node in a B-Tree consists of aalternating pointers and items.**
B-Trees consist of nodes. Each node in a B-Tree contains pointers to other nodes and items in an alternating sequence.
The items in a node are arranged sequentially in order of their keys. The key is the first value in each tuple.
A pointer to the left of an item points to another B-Tree node that contains items that are all less than the item to the right of the pointer.
A pointer to the right of an item points to a node where all the items are greater than the item.
The items in node 1 are all less than 12 while the items in node 2 are all greater than 12.

B-Trees are always balanced, meaning that all the leaf nodes appear on the same level of the tree.
A B-Tree may contain as many items and pointers as desired in each node. There will always be one more pointer than items in a node.
B-Trees don't have to fill each node. The degree of a B-Tree is the minimum number of items that a B-Tree node may contain, except for the root node.
The capacity of a node is always twice its degree. In Fig. 11.5 the degree is 2 and the capacity is 4.

**The requirements of a B-Tree are as follows:**
------------------------------------------------

1. Every node except the root node must contain between degree and 2\*degree items.

2. Every node contains one more pointer than the number of items in the node.

3. All leaf nodes are at the same level within a B-Tree.

4. The items within a B-Tree node are ordered in ascending (or descending) order.
   All nodes have their items in the same order, either ascending or descending.

5. The items in the subtree to the left of an item are all less than that item.

6. The items in the subtree to the right of an item are all greater than that item.

To maintain these properties, inserting and deleting items from the tree must be done with some care.
Inserting an item can cause splitting of a node. Deleting from a tree sometimes requires rebalancing of the tree.
Looking up an item in a B-Tree is performed much the same way lookup is performed in a binary search tree. The node is examined to find the item.
If it is not found, then the pointer is followed that lies between the items that are less than and greater than the item to be found.
If this leads to a leaf node and the item is not found in the leaf node, the item is reported as not in the tree.

**The Advantages of B-Trees**

A B-Tree may contain entire records instead of just key/value pairs as appear in Fig.
11.5 where the key/value pairs are the FeedID and record number of each record in the Feed table.
For instance, the entire record for FeedID 10 might be stored directly in the B-Tree where (10,0) currently appears.
In the examples in this text the B-Tree and the database table are stored separately.
This has the advantage that more than one B-Tree index could be built over the Feed table.
The B-Tree in Fig. 11.5 is built over the FeedID field.
Some other unique field might be used to build another B-Tree over the table if desired.
By storing the B-Tree and the table separately, multiple indices are possible.

As mentioned earlier in the chapter, B-Trees provide O(logd n) lookup time where d is the degree of the B-Tree and n is the number of items in the tree.
Hash tables provide faster lookup time than a B-Tree. So why not use a hash table instead?

Unlike a hash table, a B-Tree provides ordered sequential access to the index.
You can iterate over the items in a B-Tree much like binary trees provide iteration.
Iteration over a B-Tree provides the items or keys in ascending (or descending) order.
A hash table does not provide an ordering of its keys.

B-Trees provide O(log n) insert, delete, and lookup time as well.
While not as efficient as hash tables in this regard, B-Trees nodes are often quite large providing a very flat tree.
In this case, the time for these three operations often comes close to that of a hash table.

**B-Trees are often constructed with literally millions of items.**

When a B-Tree reaches this size, holding all the nodes in memory at one time may consume a lot of RAM.
This is a great advantage of B-Trees over hash tables. A B-Tree may be stored in a file itself.
Since files are randomly accessible on a disk, a B-Tree's node may be thought of as a record in a file.
Consider the B-Tree in Fig. 11.5. The nodes 1, 2, and 3 could be thought of as three records within a file.
The record number are the pointer values, so to search the B-Tree it is only necessary to start with the root node in memory.
Then, to search when a pointer is followed during search, the record corresponding to the new node is read into memory during the search.
A search can proceed in this way, reading one record at a time from disk.
Typically a pool of records would be held in memory for a B-Tree and records would be replaced in memory using some sort of node replacement scheme.
In this way a fixed amount of RAM can be allocated to hold a B-Tree that would typically be much smaller than the total size of the tree.
In addition, since a B-Tree can be stored in a file, it is not necessary to reconstruct the B-Tree each time it is needed.
The code in Sect. 11.2.7 stores the B-Trees in two files named Feed.idx and FeedAttribType.idx and reads the index
from the file the next time the program is run.

Deleting a record from a table with a million records or more in it could be an expensive operation if the table has to be completely rewritten.
If sequential access to the underlying table is handled through the B-Tree or if the entire file is stored in the nodes of the B-Tree, deletion
of a row or record in the table gets much simpler.
For instance, in Fig. 11.6 the feed with FeedID of 10 remains in the Feed.tbl file, but has been deleted from the B-Tree.
If sequential access is always handled through the B-Tree, it would appear that the feed with FeedID 10 has been deleted from the table.
Deleting an item from the table in this way is a O(log n) operation while deleting by rewriting the entire file would take O(n) time.
When n is millions of records, the difference between O(log n) and O(n) is significant.

The same goes for inserting a new row or record within the Feed table.
Adding one new record to the end of a file can be done quickly, without rewriting the entire file.
When a B-Tree is used the newly inserted item automatically maintains its sorted position within the file.

To summarize, B-Trees have several characteristics that make them attractive to use in relational databases and
for providing access to large quantities or ordered data. These properties include:

• Ordered sequential access over the key value on O(n) time.

• O(log n) insert time, while maintaining the ordering of the items.

• O(log n) delete time of items within the B-Tree.

• If sequential access is handled through the B-Tree then O(log n) delete time is provided for the underlying table as well.

• B-Trees can be stored in a file and B-Tree nodes can be read on an as needed basis allowing B-Trees to be larger than available memory.

• A B-Tree index stored in a file does not have to be rebuilt each time it is needed in a program.

It is this final point that make B-Trees and their derivatives so valuable to relational database implementations.
Relational databases need B-Trees and their derivative implementations to efficiently process join operations while
also providing many of the advantages listed above.

**B-Tree Implementation**

Looking up a value in a B-Tree is relatively simple and is left as an exercise for the reader.
Inserting and deleting values are where all the action is. Alan Tharp [7] provides a great discussion of both inserting and deleting values in a B-Tree.
In this text we provide new examples and suggest both iterative and recursive implementations of both operations.

#B-Tree Insert

Inserting an item in a B-Tree involves finding the leaf node which should contain the item.
It may also involve splitting if no room is left in the leaf node.
When a leaf node reaches its capacity, which is two times its degree and a new item is being inserted,
the 2\*degree+1 items are sorted and the median value (i.e. the middle value) is promoted up the tree to the parent node.
In this way, splitting may cascade up the tree.

To see the splitting process in action, consider building the tree given in Fig. 11.5 with the keys given in this order
[10, 8, 22, 14, 12, 18, 2, 50, 15].
The first item to be inserted is the 10. When this occurs, the B-Tree is empty, consisting of one empty node.
The (10,4) item is added into that node as shown in Fig. 11.7.

The items with keys 8, 14, and 22 are inserted in a similar fashion as shown in Fig. 11.8.
The node is now full. The next item to be inserted will cause a split.

The next item inserted is a 12 causing the node to split into two nodes. The left subtree node is the original node.
The right subtree contains the new node. The middle value, 12 in this case, is promoted up to the parent.
In this case, there is no parent since we split the root node. In this special case a new root node is created to hold the promoted value.
After taking these steps, the tree appears as shown in Fig. 11.9. The three values 18, 2, and 50 are inserted resulting in the tree as shown in


When 15 is inserted B-Tree node number 2 is going to split and promote the middle value, 18 in this case, up to the parent.
This time there is room in the parent so the new item is added resulting in the tree shown in Fig. 11.11.

#Inserting 15 into the B-Tree Causes Splitting

Inserting an item causes one of two possible outcomes.
Either the leaf node has room in it to add the new item or the leaf node splits resulting in a middle value and a new node being promoted to the parent.
This suggests a recursive implementation is appropriate for inserting a new item.
The recursive algorithm is given an item to insert and returns two values, the promoted key and
the new right node if there is one and proceeds as follows.

1. If this is a leaf node and there is room for it, make room and store the item in the node.

2. Otherwise. if this is a leaf node, make a new node. Sort the new item and old items. Choose the middle item to promote to the parent.
  Take the items after the middle and put them into the new node. Return a tuple of the middle item and new right node.

3. If this is a non-leaf node, call insert recursively on the appropriate subtree.
   Consult the return value of the recursive call to see if there is a newly promoted key and right subtree.
   If so, take the appropriate action to store the new item and subtree pointer in the node.
   If there is no room to store the promoted value, split again as described in step 2.

Step 3 above automatically handles any cascading splits that must occur.
After the recursive call the algorithm looks for any promoted value and handles it by either adding it into the node or by splitting again.
An iterative version of insert would proceed in a similar manner as the recursive version except that the path to the newly inserted
item would have to be maintained on a stack.
Then, after inserting or splitting the leaf node, the stack of nodes on the path to the leaf would be popped one at a time, handling any promoted values, until the stack was emptied.

When writing insert as a recursive function it makes sense to implement it as a method of a B-Tree node class.
Then the insert method on a B-Tree class can call the recursive insert on the B-Tree node class.
In this way, if the root node is split, the B-Tree insert method can deal with this by creating a
new root node from the promoted value and the left and right subtrees.
Recall that the old root is the new left subtree in the newly created node.

**アンキット**
