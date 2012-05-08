/*
  Copyright (C) 2011, 2012 by the authors of the ASPECT code.

  This file is part of ASPECT.

  ASPECT is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2, or (at your option)
  any later version.

  ASPECT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with ASPECT; see the file doc/COPYING.  If not see
  <http://www.gnu.org/licenses/>.
*/
/*  $Id$  */


#include <aspect/postprocess/visualization.h>
#include <aspect/simulator.h>

#include <deal.II/numerics/data_out.h>


namespace aspect
{
  namespace Postprocess
  {
    namespace VisualizationPostprocessors
    {
      /**
       * A class derived from DataPostprocessor that takes puts the
       * number of the parallel process (i.e., the partition number) for
       * each cell into the output file.
       *
       * The member functions are all implementations of those declared in the base
       * class. See there for their meaning.
       */
      template <int dim>
      class Partition
        : public DataPostprocessorScalar<dim>,
          public SimulatorAccess<dim>,
          public Interface<dim>
      {
        public:
          Partition ();

          virtual
          void
          compute_derived_quantities_vector (const std::vector<Vector<double> >              &uh,
                                             const std::vector<std::vector<Tensor<1,dim> > > &duh,
                                             const std::vector<std::vector<Tensor<2,dim> > > &dduh,
                                             const std::vector<Point<dim> >                  &normals,
                                             const std::vector<Point<dim> >                  &evaluation_points,
                                             std::vector<Vector<double> >                    &computed_quantities) const;
      };


      template <int dim>
      Partition<dim>::
      Partition ()
        :
        // we don't need to know about any of the solution values
        // in order to determine the partition number. thus, no
        // need to specify any update flags
        DataPostprocessorScalar<dim> ("partition",
                                      update_default)
      {}



      template <int dim>
      void
      Partition<dim>::
      compute_derived_quantities_vector (const std::vector<Vector<double> >              &uh,
                                         const std::vector<std::vector<Tensor<1,dim> > > &duh,
                                         const std::vector<std::vector<Tensor<2,dim> > > &dduh,
                                         const std::vector<Point<dim> >                  &normals,
                                         const std::vector<Point<dim> >                  &evaluation_points,
                                         std::vector<Vector<double> >                    &computed_quantities) const
      {
        Assert (computed_quantities[0].size() == 1, ExcInternalError());

        for (unsigned int q=0; q<computed_quantities.size(); ++q)
          {
            // simply get the partition number from the triangulation
            computed_quantities[q](0) = this->get_triangulation().locally_owned_subdomain();
          }
      }
    }
  }
}


// explicit instantiations
namespace aspect
{
  namespace Postprocess
  {
    namespace VisualizationPostprocessors
    {
      ASPECT_REGISTER_VISUALIZATION_POSTPROCESSOR(Partition,
                                                  "partition",
                                                  "A visualization output object that generates output "
                                                  "for the parallel partition that every cell of the "
                                                  "mesh is associated with.")
    }
  }
}