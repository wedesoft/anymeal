#include <cassert>
#include "recode.hh"


using namespace std;

Recoder::Recoder(const char *request_string) {
  m_outer = recode_new_outer(false);
  assert(m_outer);
  m_request = recode_new_request(m_outer);
  bool result = recode_scan_request(m_request, request_string);
  assert(result);
}

Recoder::~Recoder(void) {
  recode_delete_request(m_request);
  recode_delete_outer(m_outer);
}

string Recoder::process(std::string &text) {
  char *output = NULL;
  size_t output_length = 0;
  size_t output_allocated = 0;
  bool result = recode_buffer_to_buffer(m_request, text.c_str(), text.length(), &output, &output_length, &output_allocated);
  if (!result)
    throw recode_exception();
  return string(output, output_length);
}
