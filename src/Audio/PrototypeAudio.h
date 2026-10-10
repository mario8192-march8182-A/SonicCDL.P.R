#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace sclpr::audio {

struct PrototypeTrack {
    std::string id;
    std::string name;
    std::string filePath;
    std::string originalGame;
    std::string region;
    std::string notes;
    bool isPrototype{false};
    bool hasLoop{false};
};

class PrototypeAudioBank {
public:
    PrototypeAudioBank() = default;
    ~PrototypeAudioBank() = default;

    PrototypeAudioBank(const PrototypeAudioBank&) = delete;
    PrototypeAudioBank& operator=(const PrototypeAudioBank&) = delete;

    // Loads a simple manifest file for prototype tracks.
    // Supported format:
    //  id=proto_001
    //  name=Prototype Title Theme
    //  file=Audio/Prototype/ProtoTitle.ogg
    //  original_game=Sonic CD
    //  region=US
    //  has_loop=true
    //  is_prototype=true
    //  notes=Early build music
    //  ---
    bool LoadManifest(const std::string& manifestPath);

    void AddTrack(const PrototypeTrack& track);
    const PrototypeTrack* FindTrack(const std::string& id) const;
    std::vector<PrototypeTrack> GetTracks() const;

private:
    std::vector<PrototypeTrack> m_tracks;
    std::unordered_map<std::string, size_t> m_index;
};

} // namespace sclpr::audio
