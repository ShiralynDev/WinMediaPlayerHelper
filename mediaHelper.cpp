#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Media.Control.h>
#include <iostream>
#include <nlohmann/json.hpp>

using namespace winrt;
using namespace Windows::Media::Control;
using namespace Windows::Foundation;

void updateMedia() {
    init_apartment();

    auto manager = GlobalSystemMediaTransportControlsSessionManager::RequestAsync().get();
    auto session = manager.GetCurrentSession();

    nlohmann::json mediaJson;

    if (!session) {
        mediaJson["title"] = "N/A";
        mediaJson["author"] = "N/A";
        mediaJson["isPlaying"] = false;
    } else {
        auto info = session.TryGetMediaPropertiesAsync().get();
        auto playback = session.GetPlaybackInfo();

        mediaJson["title"] = info.Title();
        mediaJson["author"] = info.Artist();
        mediaJson["isPlaying"] = (playback.PlaybackStatus() == GlobalSystemMediaTransportControlsSessionPlaybackStatus::Playing);
    }

    std::cout << mediaJson.dump() << std::endl;
}

void togglePlayPause() {
    init_apartment();
    auto manager = GlobalSystemMediaTransportControlsSessionManager::RequestAsync().get();
    auto session = manager.GetCurrentSession();
    if (session) session.TryTogglePlayPauseAsync();
}

void nextSong() {
    init_apartment();
    auto manager = GlobalSystemMediaTransportControlsSessionManager::RequestAsync().get();
    auto session = manager.GetCurrentSession();
    if (session) session.TrySkipNextAsync();
}

void previousSong() {
    init_apartment();
    auto manager = GlobalSystemMediaTransportControlsSessionManager::RequestAsync().get();
    auto session = manager.GetCurrentSession();
    if (session) session.TrySkipPreviousAsync();
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: mediaHelper.exe <status|playpause|next|prev>\n";
        return 1;
    }

    std::string arg = argv[1];

    try {
        if (arg == "status") updateMedia();
        else if (arg == "playpause") togglePlayPause();
        else if (arg == "next") nextSong();
        else if (arg == "prev") previousSong();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}