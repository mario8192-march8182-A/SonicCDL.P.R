#include "Audio/PrototypeAudio.h"

#include <fstream>
#include <sstream>
#include <algorithm>

namespace sclpr::audio {

namespace {

std::string Trim(const std::string& input) {
    const auto begin = input.find_first_not_of(" \t\r\n");
    if (begin == std::string::npos) {
        return {};
    }

    const auto end = input.find_last_not_of(" \t\r\n");
    return input.substr(begin, end - begin + 1);
}

bool ParseBool(const std::string& value) {
    const std::string normalized = [&]() {
        std::string out = value;
        std::transform(out.begin(), out.end(), out.begin(), [](unsigned char ch) {
            return static_cast<char>(std::tolower(ch));
        });
        return out;
    }();

    return normalized == "true" || normalized == "1" || normalized == "yes";
}

} // namespace

bool PrototypeAudioBank::LoadManifest(const std::string& manifestPath) {
    std::ifstream input(manifestPath);
    if (!input.is_open()) {
        return false;
    }

    PrototypeTrack track{};
    bool hasEntry = false;
    std::string line;

    while (std::getline(input, line)) {
        const std::string trimmed = Trim(line);
        if (trimmed.empty()) {
            if (hasEntry) {
                AddTrack(track);
                track = PrototypeTrack{};
                hasEntry = false;
            }
            continue;
        }

        if (trimmed == "---") {
            if (hasEntry) {
                AddTrack(track);
                track = PrototypeTrack{};
                hasEntry = false;
            }
            continue;
        }

        const auto delimiter = trimmed.find('=');
        if (delimiter == std::string::npos) {
            continue;
        }

        const std::string key = Trim(trimmed.substr(0, delimiter));
        const std::string value = Trim(trimmed.substr(delimiter + 1));

        if (key == "id") {
            track.id = value;
        } else if (key == "name") {
            track.name = value;
        } else if (key == "file") {
            track.filePath = value;
        } else if (key == "original_game") {
            track.originalGame = value;
        } else if (key == "region") {
            track.region = value;
        } else if (key == "notes") {
            track.notes = value;
        } else if (key == "is_prototype") {
            track.isPrototype = ParseBool(value);
        } else if (key == "has_loop") {
            track.hasLoop = ParseBool(value);
        }

        hasEntry = true;
    }

    if (hasEntry) {
        AddTrack(track);
    }

    return !m_tracks.empty();
}

void PrototypeAudioBank::AddTrack(const PrototypeTrack& track) {
    if (track.id.empty()) {
        return;
    }

    const auto existing = m_index.find(track.id);
    if (existing != m_index.end()) {
        m_tracks[existing->second] = track;
        return;
    }

    const size_t index = m_tracks.size();
    m_tracks.push_back(track);
    m_index[track.id] = index;
}

const PrototypeTrack* PrototypeAudioBank::FindTrack(const std::string& id) const {
    const auto it = m_index.find(id);
    if (it == m_index.end()) {
        return nullptr;
    }

    return &m_tracks[it->second];
}

std::vector<PrototypeTrack> PrototypeAudioBank::GetTracks() const {
    return m_tracks;
}

} // namespace sclpr::audio
