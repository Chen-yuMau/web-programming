# web-programming
web-programming exercises
contents:
hw1:
In the store there is one claw machine with good prizes therefore dozens of people want to
give it a try. Assume that there are infinite prizes in the machine, and every customer takes
the same time (assume = a time unit) to manipulate the machine once, we call it a ‘round​’.
While the machine is being used, others have to line up in sequential order. If a customer
gets his own chance, he would play a few rounds in a row to try to win the prize. If he fails,
he will leave the machine to other customers, get some rest and come back a certain time
later; Otherwise, he will leave with his prize happily and will not come back again.
Assume that every customer has his own first arrive time, continuously play round number,
rest time, and total play round number N​. A customer will successfully get his prize on
the Nth round he has played.
There is a guarantee system in the claw machine – the guarantee number G​. It means that
if no one gets the prize for continuous n playing times, the number n will be always less than
G. Remember that the guarantee number counter will be reset when anyone gets the prize,
whether by achieving G or N.
My goal is to use multi-threads to present the behavior of customers and claw machine.
