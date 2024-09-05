#ifndef ARRAY_HPP
# define ARRAY_HPP

// • Construction with no parameter: Creates an empty array.
// • Construction with an unsigned int n as a parameter: Creates an array of n elements initialized by default.
// Tip: Try to compile int * a = new int(); then display *a.
// • Construction by copy and assignment operator. In both cases, modifying either the original array or its copy after copying musn’t affect the other array.
// • You MUST use the operator new[] to allocate memory. Preventive allocation (al- locating memory in advance) is forbidden. Your program must never access non- allocated memory.
// • Elements can be accessed through the subscript operator: [ ].
// • When accessing an element with the [ ] operator, if its index is out of bounds, an
// std::exception is thrown.
// • A member function size() that returns the number of elements in the array. This
// member function takes no parameter and musn’t modify the current instance.

# include <iostream>

# define DEFAULT "\001\033[0;39m\002"
# define YELLOW "\001\033[1;93m\002"
# define DEBUG "\001\033[1;93m\002"
# define RED "\001\033[1;91m\002"
# define GREEN "\001\033[1;92m\002"
# define BLUE "\001\033[1;94m\002"
# define CYAN "\001\033[1;96m\002"
# define MAGENTA "\001\033[1;95m\002"
# define GRAY "\001\033[1;90m\002"

template <typename T>
class Array
{
	private:
		unsigned int	_size;
		T	*_array;
	public:
		Array<T>();
		~Array<T>();
		Array<T>(unsigned int n);
		Array<T>(const Array<T> & copy);
		Array<T> &operator=(const Array<T> & copy);
		T &operator[](const unsigned int index) const;
		unsigned int size() const;  

		class OutofBoundsException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{
					return ("Index is out of Bounds");
				}
		};

		class NullArrayException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{
					return ("Array is Null");
				}
		};
};

template <typename T>
std::ostream	&operator<<(std::ostream &out, Array<T> const &array);

#endif