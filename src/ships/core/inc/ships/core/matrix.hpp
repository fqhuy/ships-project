#ifndef __SHIPS_CORE_MATRIX_HPP__
#define __SHIPS_CORE_MATRIX_HPP__

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/storage.hpp>

namespace Sp {

    class MemoryModel {
        public:
            MemoryModel();

            virtual ~MemoryModel() {
            };
    };

    class SampleModel {
        public:
            SampleModel();

            virtual ~SampleModel() {
            };
    };
    //Matrix adapter to adapt various kinds of matrix like ublas, viennal ...
    //equipvalent to java.awt.image.Raster

    template<class T> class MatrixAdapter {
        public:
            typedef T ValueType;
            typedef T& Reference;
            typedef T* Pointer;
            MatrixAdapter();
            MatrixAdapter ( const int& size1, const int& size2, const MemoryModel& memory_model, const SampleModel& sample_model );

            virtual ~MatrixAdapter() {
            };
            virtual Reference GetAt ( int x, int y );
        protected:
            MemoryModel memory_model_;
            SampleModel sample_model_;
        private:
    };

    template<class T, class L, class A> class HostMatrixAdapter :
                public MatrixAdapter<T> {
        public:

        protected:

        private:
           boost::numeric::ublas::matrix<T,L,A> matrix_;
    };

    template<class T, class L, class A> class DeviceMatrixAdapter :
                public MatrixAdapter<T> {
    };

}

#endif
