#include "MP3Track.h"
#include <iostream>
#include <cmath>
#include <algorithm>

MP3Track::MP3Track(const std::string& title, const std::vector<std::string>& artists, 
                   int duration, int bpm, int bitrate, bool has_tags)
    : AudioTrack(title, artists, duration, bpm), bitrate(bitrate), has_id3_tags(has_tags) {
    #ifdef DEBUG
    std::cout << "MP3Track created: " << bitrate << " kbps\n";
    #endif
}

// ========== TODO: STUDENTS IMPLEMENT THESE VIRTUAL FUNCTIONS ==========

void MP3Track::load() {
    std::cout << "[MP3Track::load] Loading MP3: \"" << title
              << "\" at " << bitrate << " kbps...\n";
    if (has_id3_tags)
    {
        std::cout << "  → Processing ID3 metadata (artist info, album art, etc.)... \n";
    }
    else{
        std::cout << "  → No ID3 tags found. \n";
    }
    std::cout << "  → Decoding MP3 frames... \n";
    std::cout << "  → Load complete. \n"; 
}

void MP3Track::analyze_beatgrid() {
     std::cout << "[MP3Track::analyze_beatgrid] Analyzing beat grid for: \"" << title << "\"\n";
     int estimated_beats = (duration_seconds / 60.0) * bpm;
     double precision_factor = bitrate / 320.0;
     std::cout<<"  → Estimated beats: "<<estimated_beats<<"  → Compression precision factor: "<<precision_factor<<"\n";
    // TODO: Implement MP3-specific beat detection analysis
    // NOTE: Use exactly 2 spaces before each arrow (→) character

}

double MP3Track::get_quality_score() const {
    double score = ((double)bitrate / 320.0) * 100.0;
    if(has_id3_tags) {
    score += 5.0;
    }
    if(bitrate < 128) {
    score -= 10.0;
    }
    if(score > 100.0) {
    score = 100.0;
    } else if(score < 0.0) {
    score = 0.0;
    }
    // TODO: Implement comprehensive quality scoring
    // NOTE: This method does NOT print anything

    return score;
}

PointerWrapper<AudioTrack> MP3Track::clone() const {
    try {
        MP3Track* newTrack = new MP3Track(*this);
        return PointerWrapper<AudioTrack>(newTrack);
    }
    catch (const std::bad_alloc& e) {
        return PointerWrapper<AudioTrack>(); 
    }
}