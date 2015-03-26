#ifndef TRIPLET_H
#define TRIPLET_H

template<typename T, typename U, typename V>
class triplet
{
        public:
                triplet(T first, U second, V third)
                {
                        this->first = first;
                        this->second = second;
                        this->third = third;
                }

                virtual ~triplet() {}

                T first;
                U second;
                V third;
        protected:
        private:
};

#endif // TRIPLET_H
