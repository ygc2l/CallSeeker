//------------------------------------------------------------------------------
//  Copyright 2012 Chris Scott (fbscds@gmail.com)
//  Copyright 2016-2018 WavX inc. (www.wavx.ca)
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with This program.  If not, see <https://www.gnu.org/licenses/>.
//------------------------------------------------------------------------------

#ifndef SETTINGS_H
#define SETTINGS_H

struct Settings {
    bool output_audacity = false;
    bool output_summary_results_csv = true;
    bool output_summary_analyzed_files_csv = true;
};

#endif // SETTINGS_H
