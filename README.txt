
-Huffman tree:
	Tree node attributes: frequency(key), value(character), left, right(for heap), parent, lchild, rchild.(for huffman tree)

	For each symbol of frequency table, inserts a node into heap,construct a Min-heap,using node.left, node.right and node.parent to connect.
	Remove two least frequency node from heap, create a new node as the parent node of them, using node.lchild, node.rchild and node.parnet to connect.
	Then insert the parent node into heap.
	repeat, until one node left in the heap.

-Header:
	Stored the frequency table in the header.4 bytes for one characters.

-search:
	Method: KMP algorithm
	Read by byte from encoded file, decode it and stored in a char array[1024], do KMP pattern matching for every array and reused the memory.
	Reserved last length(search term)-1 characters of the next array to keep the consistency, then continue reading and decoding, until reach the end of the file.
	Do KMP pattern matching for the last array.
	Returned the sum of all the returned value of KMP.
