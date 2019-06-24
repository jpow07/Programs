# Introduction
### Definition: Algorithms - methods for solving problems suited for computer implementation



## Analysis


## Growth


## Big-O Notation


## Basic Recurrences


## Quick-Find
It checks if there is a connection between p and q by determining if id[p] != id[q]. If theyare not equal it replaces every index that is equal to the current index of id[p], called t, and will propogate this throughout the array.

Once p and q are all connected the final result will be an array with identical indicies.
For test1 the final result is an array filled with 1's

Concept: 

Fill an array where each index has the value of its current index:
	id[0] = 0, id[1] = 1, ..., id[N] = N
Then request two pairs, p and q
if p and q are not equal then index id[p] and set it to a temporary value, t

Now cycle throughout the entire array and check for all instances where id[i] == id[q]
if there is a match replace the current value at index, id[i], with the value at id[q]

This is performed until all values are equal, which signifies that all points are connected.

If p and q are equal it signifies that they are already connected so we can exclude them from the result.

Afterwards we are left with a set of numbers whose connections complete the connection, and every other pair is "junk"

#### Run-time analysis: O(M * N)
Where:
	M is the number of union operations
	N is the size of the array (length of connection)

## Quick-Union

Similar to Quick find; however, instead of checking every value we only check that value of id[i] is not equal to the value of p and we do the same for id[i] and q.
If p and q are equal we know they are already connected and can disregard these connections.
However, if they are not equal we can set id[p] equal to the value of q.

This is much faster because it doesn't require us to traverse through the entire array for  every union operation; However, the final result doesn't all equal the same value rather they show the relationship between the index and the value. 

Run test2 with quickunion and you will see that:

The value at a certain index shows a connection; therefore, id[5] == 6, so 5 is connected to 6.

id[6] == 9; therefore, 6 is connected to 9 and 5 is also connected to 9, transitively

###### Side Note: Transitivity states that if 1 is connected to 3 and 3 is connected to 5, then 1 is also connected to 5 through a transitive association. 

While this makes it much harder to see that all pairs are connected it improves the performance by 50%.

#### Run-time analysis: O( (N * M) / 2 )
Where:
	M is the number of Union operation
	N is the size of the array
Note: This cuts the performance in half from the quickfind alogirthm above.
 
However, there is a case when its worse run-time is longer than (M*N)/2, and that occurs when the points are connected seqentially; (ie 0 1, 1 2, 2 3, ..., N-2 N-1)

## Weighted Quick-Union

