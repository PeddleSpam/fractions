/*
 *  Copyright (C) 2013 Free Software Foundation, Inc.
 *
 *  matrix.h - a simple 3x3 matrix class.
 *
 *  Gaius Mulley <gaius.mulley@southwales.ac.uk>
 */

#if !defined(MATRIXH)
#  define MATRIXH

#include <cassert>
#include <cstdio>
#include <iostream>

const bool debugging = false;
const int dimensions = 3;


/*
 *  forward declaration to say that matrix is a class and also a template.
 */

// try class rather than typename
template <typename T> class matrix;
template <typename T> std::ostream& operator<< (std::ostream& os, const matrix <T>& l);

template <class T> class matrix
{
 private:
  bool is_initialised;
  T contents[dimensions][dimensions];

 public:
  matrix (void);
  ~matrix (void);
  matrix (const matrix &from);
  matrix& operator= (const matrix &from);
  friend std::ostream& operator<< <> (std::ostream& os, const matrix<T>& l);
  
  matrix (T a, T b, T c,
      T d, T e, T f,
      T g, T h, T i);

  matrix& operator* (const matrix &right);
  matrix& operator~ (void);
  matrix& operator() (void);
  void get_contents (T (&out)[dimensions][dimensions]);
  void set_contents (T in[dimensions][dimensions]);

  matrix& rotate (T r);
};


/*
 *  matrix - constructor, builds an uninitialised matrix.
 *           pre-condition:   none.
 *           post-condition:  an uninitialised matrix returned.
 */

template <class T> matrix<T>::matrix ()
  : is_initialised(false)
{
}


/*
 *  ~matrix - deconstructor for matrix.
 *            pre-condition:   none.
 *            post-condition:  matrix marked as uninitialised before
 *                             being deleted.
 */

template <class T> matrix<T>::~matrix (void)
{
  is_initialised = false;
}


/*
 *  copy operator - redefine the copy operator.
 *                  pre-condition :  a matrix.
 *                  post-condition:  a copy of the matrix.
 */

template <class T> matrix<T>::matrix (const matrix<T> &from)
{
  is_initialised = from.is_initialised;
  if (is_initialised)
    for (int j = 0; j<dimensions; j++)
      for (int i = 0; i<dimensions; i++)
    contents[j][i] = from.contents[j][i];
}


/*
 *  operator=  - redefine the assignment operator.
 *               pre-condition :  a matrix.
 *               post-condition:  a copy of the matrix.
 */

template <class T> matrix<T>& matrix<T>::operator= (const matrix<T> &from)
{
  is_initialised = from.is_initialised;
  if (is_initialised)
    for (int j = 0; j<dimensions; j++)
      for (int i = 0; i<dimensions; i++)
    contents[j][i] = from.contents[j][i];
}


/*
 *  operator*  - redefine the multiply operator.
 *               pre-condition :  a matrix.
 *               post-condition:  a copy of the matrix.
 */

template <class T> matrix<T>& matrix<T>::operator* (const matrix<T> &right)
{
  matrix left = *this;  // take a copy of ourselves before we change contents
  assert (is_initialised);
  assert (right.is_initialised);

  /* 1st row */
  contents[0][0] = ((left.contents[0][0] * right.contents[0][0]) +
            (left.contents[0][1] * right.contents[1][0]) +
            (left.contents[0][2] * right.contents[2][0]));
  contents[0][1] = ((left.contents[0][0] * right.contents[0][1]) +
            (left.contents[0][1] * right.contents[1][1]) +
            (left.contents[0][2] * right.contents[2][1]));
  contents[0][2] = ((left.contents[0][0] * right.contents[0][2]) +
            (left.contents[0][1] * right.contents[1][2]) +
            (left.contents[0][2] * right.contents[2][2]));
  /* 2nd row */
  contents[1][0] = ((left.contents[1][0] * right.contents[0][0]) +
            (left.contents[1][1] * right.contents[1][0]) +
            (left.contents[1][2] * right.contents[2][0]));
  contents[1][1] = ((left.contents[1][0] * right.contents[0][1]) +
            (left.contents[1][1] * right.contents[1][1]) +
            (left.contents[1][2] * right.contents[2][1]));
  contents[1][2] = ((left.contents[1][0] * right.contents[0][2]) +
            (left.contents[1][1] * right.contents[1][2]) +
            (left.contents[1][2] * right.contents[2][2]));
  /* 3rd row */
  contents[2][0] = ((left.contents[2][0] * right.contents[0][0]),
            (left.contents[2][1] * right.contents[1][0]),
            (left.contents[2][2] * right.contents[2][0]));
  contents[2][1] = ((left.contents[2][0] * right.contents[0][1]),
            (left.contents[2][1] * right.contents[1][1]),
            (left.contents[2][2] * right.contents[2][1]));
  contents[2][2] = ((left.contents[2][0] * right.contents[0][2]),
            (left.contents[2][1] * right.contents[1][2]),
            (left.contents[2][2] * right.contents[2][2]));
}


/*
 *  matrix - constructor.
 *           pre-condition:  none.
 *           post-condition: an initialised matrix.
 */

template <class T> matrix<T>::matrix (T a, T b, T c,
                      T d, T e, T f,
                      T g, T h, T i)
{
  is_initialised = true;
  contents[0][0] = a;
  contents[0][1] = b;
  contents[0][2] = c;

  contents[1][0] = d;
  contents[1][1] = e;
  contents[1][2] = f;

  contents[2][0] = g;
  contents[2][1] = h;
  contents[2][2] = i;
}


/*
 *  get_contents - pre-condition:  an initialised matrix.
 *                 post-condition: copies content of the matrix into, out.
 */

template <class T> void matrix<T>::get_contents (T (&out)[dimensions][dimensions])
{
  assert (is_initialised);
  out[0][0] = contents[0][0];
  out[0][1] = contents[0][1];
  out[0][2] = contents[0][2];

  out[1][0] = contents[1][0];
  out[1][1] = contents[1][1];
  out[1][2] = contents[1][2];

  out[2][0] = contents[2][0];
  out[2][1] = contents[2][1];
  out[2][2] = contents[2][2];
}


/*
 *  set_contents - pre-condition:  none.
 *                 post-condition: copies, in, into the matrix.
 */

template <class T> void matrix<T>::set_contents (T in[dimensions][dimensions])
{
  assert (is_initialised);
  contents[0][0] = in[0][0];
  contents[0][1] = in[0][1];
  contents[0][2] = in[0][2];

  contents[1][0] = in[1][0];
  contents[1][1] = in[1][1];
  contents[1][2] = in[1][2];

  contents[2][0] = in[2][0];
  contents[2][1] = in[2][1];
  contents[2][2] = in[2][2];
}


/*
 *  operator~ - call approximate for all values held in matrix.
 *              pre-condition :  an initialised matrix.
 *              post-condition:  all values will have been assigned with ~
 *                               itself.
 */

template <class T> matrix<T>& matrix<T>::operator~ (void)
{
  assert (is_initialised);
  for (int j = 0; j<dimensions; j++)
    for (int i = 0; i<dimensions; i++)
      contents[j][i] = ~contents[j][i];
}


/*
 *  operator() - define a function call to mean simplify all contents of
 *               values held in matrix.
 *               pre-condition :  an initialised matrix.
 *               post-condition:  all values will have been assigned with
 *                                result of calling () on itself.
 */

template <class T> matrix<T>& matrix<T>::operator() (void)
{
  assert (is_initialised);
  for (int j = 0; j<dimensions; j++)
    for (int i = 0; i<dimensions; i++)
      contents[j][i] = contents[j][i]();

  return *this;
}


/*
 *  rotate - assigns contents of the matrix to represent a rotation.
 *           pre-condition :  none.
 *           post-condition:  sets contents to a Matrix representing the rotation
 *                            transformation about 0, 0 with r radians.
 */

template <class T> matrix<T>& matrix<T>::rotate (T r)
{
  is_initialised = true;

  contents[0][0] = r.cos (); 
  contents[0][1] = -r.sin (); 
  contents[0][2] = (T)(0);
  contents[1][0] = r.sin ();
  contents[1][1] =  r.cos ();
  contents[1][2] = (T)(0);
  contents[2][0] = (T)(0);
  contents[2][1] =  (T)(0);
  contents[2][2] = (T)(1);

  return *this;
}


/*
 *   operator<<  - shift left (output) operator.
 *                 pre-condition :  a matrix.
 *                 post-condition:  matrix printed.
 */

template<class T>
std::ostream& operator<< (std::ostream& os, const matrix<T>& l)
{
  if (l.is_initialised)
    {
      os << "+----\n";
      for (int j = 0; j<dimensions; j++)
    {
      os << "| " ;
      for (int i = 0; i<dimensions; i++)
        {
          os << ( ( l.contents[j][i] ).evaluate() ).toLongDouble();
          if (i < dimensions-1)
        os << ", ";
        }
      os << "\n";
    }
      os << "+----\n";
    }
  else
    os << "uninitialised matrix\n";

  return os;
}

#endif
