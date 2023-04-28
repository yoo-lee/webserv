#ifndef RESULT_H
#define RESULT_H

template <typename T, typename E> class Result
{
  private:
    T _ok;
    E _err;
    book _is_ok;

  public:
    Result<T, E>(/* args */);
    T Ok();
    E Err();
    ~Result();
};
template <typename T, typename E> Result::Result<T, E>(/* args */)
{
}

Result::~Result()
{
}

#endif /* RESULT_H */
