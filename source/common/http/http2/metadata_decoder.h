#pragma once

#include <cstdint>
#include <string>

#include "envoy/http/codec.h"

#include "common/buffer/buffer_impl.h"
#include "common/common/logger.h"

#include "nghttp2/nghttp2.h"

namespace Envoy {
namespace Http {
namespace Http2 {

class MetadataEncoderDecoderTest_VerifyEncoderDecoderOnMultipleMetadataMaps_Test;

// A class that decodes METADATA payload in the format of HTTP/2 header block into MetadataMap, a
// map of string key value pairs.
class MetadataDecoder : Logger::Loggable<Logger::Id::http2> {
public:
  /**
   * @param metadata_map is where to save the decoded metadata.
   */
  MetadataDecoder(MetadataCallback cb, MetadataMap& metadata_map);

  /**
   * Calls this function when METADATA frame payload is received. The payload doesn't need to be
   * complete.
   * @param data is the pointer to the start of the payload.
   * @param len is the size of the received payload.
   * @return whether Metadata is received successfully.
   */
  bool receiveMetadata(const uint8_t* data, size_t len);

  /**
   * Calls when a complete METADATA frame is received. The function will decode METADATA received.
   * If the frame is the last one in the group, the function triggers the registered callback
   * function callback_.
   * @param end_metadata indicates if all the METADATA has been received.
   * @return whether the operation succeeds.
   */
  bool onMetadataFrameComplete(bool end_metadata);

  /**
   * @return payload_.
   */
  Buffer::OwnedImpl& payload() { return payload_; }

private:
  friend class MetadataEncoderDecoderTest_VerifyEncoderDecoderOnMultipleMetadataMaps_Test;
  /**
   * Decodes METADATA payload using nghttp2.
   * @param end_metadata indicates is END_METADATA is true.
   * @return if decoding succeeds.
   */
  bool decodeMetadataPayloadUsingNghttp2(bool end_metadata);

  // Metadata event callback function.
  MetadataCallback callback_;

  // Payload received.
  Buffer::OwnedImpl payload_;

  // Metadata that is currently under decoding.
  MetadataMap& metadata_map_;

  // Payload size limit. If the payload received exceeds the limit, fails the connection.
  const uint64_t max_payload_size_bound_ = 1024 * 1024;

  // TODO(soya3129): consider sharing the inflater with all streams in a connection. Caveat:
  // inflater failure on one stream can impact other streams.
  typedef CSmartPtr<nghttp2_hd_inflater, nghttp2_hd_inflate_del> Inflater;
  Inflater inflater_;
};

} // namespace Http2
} // namespace Http
} // namespace Envoy
