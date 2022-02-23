# Simple Parallelization Exercises
The cpp files consist of simple exercise implemented in C++ to parallelize tasks, this way it is possible to perform these tasks faster using a determined number of threads from our proccessor.

The file ``find_prime_nums.cpp`` is an algorithm that finds the prime numbers, larger than a given value, in an array. Setting the parameters, an array is randomly initialized with integer values within a range. Then, each thread looks for the prime numbers larger than the given value in its corresponding set of numbers, and finally they're printed.

On the other hand, the file ``pi_taylor.cpp`` approximates _pi_ using the taylor series shown in the figure. As we increase the number of steps, i.e. the number of terms in the series, _pi_ will be more accurate. The parameter to increase the number of terms in the series is the variable ``steps``.

<img src="https://user-images.githubusercontent.com/71872419/155395489-da780d9a-d028-4981-ad08-f83d12fff76d.PNG" width="420" height="180">
