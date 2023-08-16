HOMEWORK 9: MINIBLAST


NAME:  < Aaron Verkleeren >


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

< Danny Zhou, DS Lab 11 >

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  < 10 >


HASH FUNCTION DESCRIPTION
I used a similar hash function to the one given in lab. I slightly altered it by seperating
the strings by base pair and location in string.





HASH TABLE IMPLEMENTATION
My implement of the hash table was a std::list of pairs, where the first item in the pair
was a string relating to the key and the second a vector of all the strings locations.






ANALYSIS OF PERFORMANCE OF YOUR ALGORITHM:
(order notation & concise paragraph, < 200 words)

L = length of the genome sequence
q - query length
p - number of different locations where key is found
k - key size (k-mer size)

How much memory will the hash table data structure require (order notation for memory use)?
    
    For my hash table, I looped through the entire genome sequence and stored each key along with
    a vector of all the locations that key was found at. Therefore the max amount of keys there could be
    would be 4^k and since some of the keys were repeated my memory usage would be O((4^k)*p).


What is the order notation for performance (running time) of each of
the commands? What about for the hash table's insert, find, and resize?
    1. The genome command requires that you read the entire genome file, so it would be O(L)
    2. occupancy just gives a number which is O(1)
    3. table_size also just gives a number which is O(1)
    4. "kmer" command is followed by table building. Insert() function is called
        O(4^k) times. Each call need to hash the key which is O(1), find suitable
        position which is O(p), and append position to the end of position which is also
        O(1). Cost of insert is O(p). Hence, the performance of building hash table is
        O((4^k)*p).
    5. "query" command calls the similars function. Similars loops through all the different
        starting positions of the key, O(p). And also checks each base pair for q length which is O(q).
        then at the end there is a checker which is O(q). Overall O(p*q^2).
    6. insert as previously stated is O(p).
    7. find uses the hash function O(1), and std::find O(p), so find is O(p)
    8. resize remakes all the items in the hash table into a new one, so it is O((4^k)*p)



EXTRA CREDIT
Add a new command to implement the database using one of the other
 data structures that we have covered so far in the course: 
vectors, lists, arrays etc. 

Compare the performance your alternative method to the homework method
by making a table of run times for each of the genomes and query 
sets provided with the homework  and compare the times to build
the index and the times to process the queries. 
Document any new commands you have added in your README file.



MISC. COMMENTS TO GRADER:  
(optional, please be concise!)


