# pod-racing-cpp
Pod racing game code

This is my try at the pod racing game algorithm written in c++.

It is an interesting kinematics problem that i'm sure has an exact mathematical solution (or multiple) but unfortunately i did not have much time this week 
to think about it throroughly. The optimal path for the pod should be AFAIK determined with some multivariable calculus as long as collisions and 
opponent AI are not involved.

My approach was rather simple. The first lap was quite slow, simply to pass through all the points in the circuit. Once all the points had been
passed at least once, The pod could apply presteering to orient itself towards the next checkpoint. 

The CodInGame environment was rather annoying to work in due to the inability to attach a debugger and step through the code line-by-line.
In order to plot the optimal path one would need to know the decelaration coefficient of the pod, whether it is dependent on the speed, turn rate 
and so on, which take lots of work to reliably gauge.

Overall it was a perplexing assignment and I wish i had a bit more time to research more topics related to AI and mathematics to work it out.
