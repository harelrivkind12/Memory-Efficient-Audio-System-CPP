#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>
#include <algorithm>

DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist) {}

/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
    DJLibraryService::~DJLibraryService(){
        for (AudioTrack* track : library)
        {
            delete track;
        } 
    }

void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    for (const SessionConfig::TrackInfo& track : library_tracks)
    {
        if (track.type == "MP3")
        {
         MP3Track* new_track = new MP3Track(track.title,track.artists,track.duration_seconds,track.bpm,track.extra_param1,track.extra_param2);
         library.push_back(new_track);
          std::cout << "MP3: MP3Track created: " << new_track->get_title() << ", " 
                          << track.extra_param1 << " kbps\n";
        }
        else if (track.type == "WAV") {
           WAVTrack* new_track = new WAVTrack(track.title, track.artists, track.duration_seconds, track.bpm, track.extra_param1, track.extra_param2);
           library.push_back(new_track);
           std::cout << "WAV: WAVTrack created: " << new_track->get_title() << ", " 
                          << track.extra_param1 << "Hz/" << track.extra_param2<< "bit\n";
        }
    }
    std::cout << "[INFO] Track library built: " << library_tracks.size() << " tracks loaded\n";
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    return playlist.find_track(track_title);
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
      std::cout << "[INFO] Loading playlist: " << playlist_name << "\n";   
     // playlist.clean(playlist_name);
      playlist = (playlist_name);
      int count = 0;
      for (const int& index : track_indices){
       if (index < 1 || index >= library.size())
       {
        std::cout << "[WARNING] Invalid track index: " << index << "\n";
            continue;
       }
        AudioTrack* track = library[index];
        PointerWrapper<AudioTrack> cloned_wrapper = track->clone();
        if (!cloned_wrapper)
        {
            std::cout << "[ERROR] Track: \"" <<track->get_title() << "\" failed to clone\n";
            continue;
        }
        cloned_wrapper ->load();
        cloned_wrapper ->analyze_beatgrid();
        playlist.add_track(cloned_wrapper.release());
        std::cout << "Added '" << cloned_wrapper->get_title() << "' to playlist '" << playlist_name << "'\n";
        count++;
      }
        std::cout << "[INFO] Playlist loaded: " << playlist_name << " (" << count << " tracks)\n";                                     
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    std::vector<std::string> titles;
    std::vector<AudioTrack*> tracks = playlist.getTracks();
    for(AudioTrack* track : tracks){
        titles.push_back(track->get_title());
    }
    std::reverse(titles.begin(), titles.end());
    return titles;
}
