//
// base64.h
//

#ifndef _HADOOP_SZL_BASE64_H_
#define _HADOOP_SZL_BASE64_H_

#include <string>


bool Base64Decode(const std::string& src, std::string* dest);
bool Base64Encode(const std::string& src, std::string* dest);

#endif /* _HADOOP_SZL_BASE64_H_ */
