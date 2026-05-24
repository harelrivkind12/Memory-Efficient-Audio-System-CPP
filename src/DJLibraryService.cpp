#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>
#include <algorithm>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist), library() {}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
DJLibraryService:: ~DJLibraryService(){
    for (AudioTrack* track : library) {
        delete track;
}
}
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    for(const SessionConfig::TrackInfo& track_current:library_tracks){
        if(track_current.type=="MP3"){
            MP3Track* curr=new MP3Track(track_current.title,track_current.artists, track_current.duration_seconds,track_current.bpm,track_current.extra_param1,track_current.extra_param2);
            library.push_back(curr);
            std::cout<<"MP3Track created: "<<track_current.extra_param1<<" kbps\n";
        }
        else{
            WAVTrack* curr = new WAVTrack(track_current.title,track_current.artists, track_current.duration_seconds,track_current.bpm,track_current.extra_param1,track_current.extra_param2);
            library.push_back(curr);
            std::cout<<"WAVTrack created: "<<track_current.extra_param1<<"Hz/"<<track_current.extra_param2<<"bit\n";
        }
    }
    std::cout<<"[INFO] Track library built: "<<library_tracks.size()<<" tracks loaded\n";
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
    std::cout << "[INFO] Loading playlist: " << playlist_name << std::endl;
    // playlist.reset(playlist_name); //helper function to reset playlist
    playlist = Playlist(playlist_name);
    int counter = 0; //Counting tracks added
    int size = library.size();
    for(int lib_index : track_indices){
        if(lib_index < 1 || lib_index > size){
            std::cout << "[WARNING] Invalid track index: " << lib_index << std::endl;
            
        }
        else{
            PointerWrapper<AudioTrack> clone_track = library[lib_index-1]->clone();
            AudioTrack* clonePtr = clone_track.release();
            if(clonePtr==nullptr){
                std::cout<< "[ERROR] Track: failed to clone " << std::endl;
            }
            else{
                clonePtr->load();
                clonePtr->analyze_beatgrid();
                playlist.add_track(clonePtr);
                counter++;
                std::cout << "Added '" << clonePtr->get_title() << "' to playlist '" << playlist_name << "'" << std::endl;
            }

        }
        std::cout << "[INFO] Playlist loaded: " << playlist_name << " (" << counter << " tracks)" << std::endl;
    }
    
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    std::vector<std::string> titles;
    std::vector<AudioTrack*>tracks=playlist.getTracks();
    for(AudioTrack* trackptr:tracks){
        titles.push_back(trackptr->get_title());
    }
    std::reverse(titles.begin(), titles.end());
    return titles;
}