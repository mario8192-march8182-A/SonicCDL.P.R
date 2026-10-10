#include "Audio/PrototypeAudioManager.h"

namespace sclpr::audio {

void PrototypeAudioManager::RegisterTrack(const ProtoTrackMeta& track) {
    m_tracks.push_back(track);
}

const ProtoTrackMeta* PrototypeAudioManager::FindById(const std::string& id) const {
    for (const auto& track : m_tracks) {
        if (track.id == id) {
            return &track;
        }
    }
    return nullptr;
}

const std::vector<ProtoTrackMeta>& PrototypeAudioManager::GetAll() const noexcept {
    return m_tracks;
}

} // namespace sclpr::audio
