// Copyright (c) 1999,2001  Utrecht University (The Netherlands),
// ETH Zurich (Switzerland), Freie Universitaet Berlin (Germany),
// INRIA Sophia-Antipolis (France), Martin-Luther-University Halle-Wittenberg
// (Germany), Max-Planck-Institute Saarbruecken (Germany), RISC Linz (Austria),
// and Tel-Aviv University (Israel).  All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the License.
// See the file LICENSE.LGPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.5-branch/STL_Extension/include/CGAL/Twotuple.h $
// $Id: Twotuple.h 44546 2008-07-28 16:49:46Z spion $
// 
//
// Author(s)     : Andreas Fabri

#ifndef CGAL_TWOTUPLE_H
#define CGAL_TWOTUPLE_H

#ifndef CGAL_NO_DEPRECATED_CODE

CGAL_BEGIN_NAMESPACE

template < class T >
struct Twotuple
{
  typedef T value_type;

  T  e0, e1;

  Twotuple() CGAL_DEPRECATED
  {}

  Twotuple(const T & a0, const T &a1) CGAL_DEPRECATED : e0(a0), e1(a1)
  {}
};

CGAL_END_NAMESPACE

#endif // CGAL_NO_DEPRECATED_CODE

#endif // CGAL_TWOTUPLE_H
