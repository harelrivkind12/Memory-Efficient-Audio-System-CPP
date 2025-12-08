#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    bool exists = cache.contains(track.get_title());
    if(exists){
        cache.get(track.get_title());
        return 1;
    }
    PointerWrapper<AudioTrack> copyTrack(track.clone());
    AudioTrack* clonePtr = copyTrack.release();
    if(!clonePtr){
        std::cout << "Error: Track Cloning failed for [DJControllerService]" << clonePtr->get_title() <<"\n";
        return 0;
    }
    clonePtr->load();
    clonePtr->analyze_beatgrid();
    PointerWrapper<AudioTrack> newPtr(clonePtr);
    bool put = cache.put(std::move(newPtr));
    if(put){
        newPtr.release();
        return -1;
    }
    return 0;
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
   return cache.get(track_title);   
}