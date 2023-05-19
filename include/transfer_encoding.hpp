#ifndef TRANSFER_ENCODING_H
#define TRANSFER_ENCODING_H

#include <stdexcept>
#include <string>

using std::string;

class TransferEncoding
{
  private:
    const std::string TRANSFER_ENCODING_TYPE_STRS[5] = {
        "chunked", "compress", "deflate", "gzip", "identity",
    };
    const int TYPE_COUNT = 5;

  public:
    typedef enum E_TRANSFER_ENCODING_TYPE {
        CHUNKED,
        COMPRESS,
        DEFLATE,
        GZIP,
        IDENTITY
    } TRANSFER_ENCODING_TYPE;

    TRANSFER_ENCODING_TYPE _type;

    TransferEncoding();
    TransferEncoding(string transfer_encoding_str);
    TRANSFER_ENCODING_TYPE get_type() const;
    string get_transfer_encoding_str() const;

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
