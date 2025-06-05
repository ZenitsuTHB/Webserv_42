/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScopedPtr.tpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolcy <avolcy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 18:25:31 by avolcy            #+#    #+#             */
/*   Updated: 2025/06/05 19:24:44 by avolcy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// The ScopedPtr is a simple yet powerful smart pointer that provides automatic 
// memory management in C++98, where modern smart pointers like std::unique_ptr 
// aren't available. Let me explain why this is so valuable for your server code.

// Problem: Code was manually manages Server instances with raw pointers
// Solution: ScopedPtr automatically deletes the owned object when it goes out of scope
// Impact: Eliminates memory leaks if exceptions occur or you forget to delete

#ifndef SCOPEDPTR_TPP
# define SCOPEDPTR_TPP

#include <iostream>

template < typename T>
class SmartPtr {
    
    T* ptr;
    public:
        explicit SmartPtr(T* p = 0) : ptr(p) {}
        ~SmartPtr() { delete ptr; }

        //getter
        T*  getPtr() const { return ptr;} 
        // Dereference operators
        T*  operator->() const { return ptr; }
        T&  operator*() const { return *ptr; }

        //Management Methods
        void    reset(T* p = 0) { delete ptr; ptr = p; }
        T*  release() { T* p = ptr; ptr = 0; return p; }
    private:
        SmartPtr(const SmartPtr & );
        SmartPtr & operator = (const SmartPtr & );
};

#endif