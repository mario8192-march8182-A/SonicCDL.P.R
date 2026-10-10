#pragma once

#include <string>
#include <vector>

namespace sclpr::audio {

struct ProtoTrackMeta {
    std::string id;
    std::string title;
    std::string filePath;
    std::string source;
    std::string notes;
    bool prototype{false};
    bool looped{false};
};

class PrototypeAudioManager {
public:
    PrototypeAudioManager() = default;
    ~PrototypeAudioManager() = default;

    void RegisterTrack(const ProtoTrackMeta& track);
    const ProtoTrackMeta* FindById(const std::string& id) const;
    const std::vector<ProtoTrackMeta>& GetAll() const noexcept;

private:
    std::vector<ProtoTrackMeta> m_tracks;
};

} // namespace sclpr::audio
