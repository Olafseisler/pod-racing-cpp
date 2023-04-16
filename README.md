# pod-racing-cpp
Pod racing game code

This is my try at the pod racing game algorithm written in c++.

It is an interesting kinematics problem that i'm sure has an exact mathematical solution (or multiple) but unfortunately i did not have much time this week 
to think about it throroughly. The optimal path for the pod should be AFAIK determined with some multivariable calculus as long as collisions and 
opponent AI are not involved.

My approach was rather simple. The first lap was quite slow, simply to pass through all the points in the circuit. Once all the points had been
passed at least once, The pod could apply presteering to orient itself towards the next checkpoint. 

I wrote a function that used a sigmoid curve to vary the thrust by distance from the next chekpoint and a gaussian function to regulate the speed relative to the angle between the pod and point. I also made sure it always had some base thrust to move towards the next objective.

The thrust vs. relative distance coefficient:

![image](https://user-images.githubusercontent.com/49180643/232294800-1e8de53a-bb29-4642-9e73-dcd154c16d9c.png)

Thrust vs. angle combined with distance coefficient:

![image](https://user-images.githubusercontent.com/49180643/232295023-a294197a-cce5-41c6-b308-52894c2601c6.png)

The CodInGame environment was rather annoying to work in due to the inability to attach a debugger and step through the code line-by-line.
In order to plot the optimal path one would need to know the decelaration coefficient of the pod, whether it is dependent on the speed, turn rate 
and so on, which take lots of work to reliably gauge.

Overall it was a perplexing assignment and I wish i had a bit more time to research more topics related to AI and mathematics to work it out.
