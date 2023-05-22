#ifndef TRANSFER_ENCODING_H
#define TRANSFER_ENCODING_H

#include <stdexcept>
#include <string>

using std::string;

class TransferEncoding
{
  private:
    static const char* TRANSFER_ENCODING_TYPE_STRS[];
    static const int TYPE_COUNT = 6;

  public:
    typedef enum E_TRANSFER_ENCODING_TYPE {
        CHUNKED,
        COMPRESS,
        DEFLATE,
        GZIP,
        IDENTITY,
        NONE
    } TRANSFER_ENCODING_TYPE;

    TRANSFER_ENCODING_TYPE _type;

    TransferEncoding();
    TransferEncoding(string transfer_encoding_str);
    TRANSFER_ENCODING_TYPE get_type() const;
    string get_str() const;

    TransferEncoding const& operator=(TransferEncoding const& x);
    bool operator==(string type) const;
    bool operator==(TransferEncoding te) const;
    bool operator==(TransferEncoding::TRANSFER_ENCODING_TYPE te) const;
    bool operator!=(string type) const;
    bool operator!=(TransferEncoding te) const;
    bool operator!=(TransferEncoding::TRANSFER_ENCODING_TYPE te) const;
    ~TransferEncoding();
};

#endif /* TRANSFER_ENCODING_H */
