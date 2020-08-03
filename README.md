# Test-Selection-and-Prioritization-with-Dynamic-Programming (Check my Report = Rapor)

First question was a variant of knapsack problem, total running time was the capacity of the
sack, amount of bugs were the item values and the running times were the weights. To solve
the problem a dynamic programming solution is implemented using the table method where
I go increase the capacity iteratively and change or add items if necessary, as described in the
lecture slides.

Since this is knapsack problem it works with integer values, yet a solution can be proposed
for real values. Here of course I will not be getting an exact optimum solution but a
pseudo-optimum. Let us say the running times are rounded to the second decimal place for
example if a running time is 4.123123 I will take it as 4.12, then I can take 0.01 as our
discrete smallest weight, thus evething will be converted to 0.01 units, meaning I will
divide each weight value and our capacity to 0.01 then round it. After doing this I can run
the same knapsack algorithm and find a close enough solution.

Second question is an edit distance question which is again a common dynamic programming
problem.I used Levenshtein Distance algorithm. In the implementation distances between
spesific locations on the array is kept in memory so while I continue calculating I don’t
constantly calculate the value for already calculated operations. I am basically countring the
number of remove and adds to make the two given arrays the same.



Importent Note!
In the text file (input file) Sometimes TAB sometimes Space has been used.In order to get rid
of any problem ı have chanced all tabs into space.
It is working both texts but please used the one I send in the folder
