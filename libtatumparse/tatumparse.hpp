#ifndef TATUMPARSE_H
#define TATUMPARSE_H
/*
 * libtatumparse - Kevin E. Murray 2016
 *
 * Released under MIT License see LICENSE.txt for details.
 *
 * OVERVIEW
 * --------------------------
 * This library provides support for parsing Berkely Logic Interchange Format (TATUM)
 * files. It supporst the features required to handle basic netlists (e.g. .model, 
 * .inputs, .outputs, .subckt, .names, .latch)
 *
 * USAGE
 * --------------------------
 * Define a callback derived from the tatumparse::Callback interface, and pass it
 * to one of the tatumparse::tatum_parse_*() functions.
 *
 * The parser will then call the various callback methods as it encouters the 
 * appropriate parts of the netlist.
 *
 * See main.cpp and tatum_pretty_print.hpp for example usage.
 *
 */
#include <vector>
#include <string>
#include <memory>
#include <limits>
#include <functional>

namespace tatumparse {
/*
 * Data structure Forward declarations
 */
enum class NodeType {
    SOURCE,
    SINK,
    IPIN,
    OPIN
};

enum class TagType {
    SETUP_DATA,
    SETUP_CLOCK,
    HOLD_DATA,
    HOLD_CLOCK
};

/*
 * Callback object
 */
class Callback {
    public:
        virtual ~Callback() {};

        //Start of parsing
        virtual void start_parse() = 0;

        //Sets current filename
        virtual void filename(std::string fname) = 0;

        //Sets current line number
        virtual void lineno(int line_num) = 0;

        virtual void start_graph() = 0;
        virtual void add_node(int node_id, NodeType type, std::vector<int> in_edge_ids, std::vector<int> out_edge_ids) = 0;
        virtual void add_edge(int edge_id, int src_node_id, int sink_node_id) = 0;
        virtual void finish_graph() = 0;

        virtual void start_constraints() = 0;
        virtual void add_clock_domain(int domain_id, std::string name) = 0;
        virtual void add_clock_source(int node_id, int domain_id) = 0;
        virtual void add_input_constraint(int node_id, float constraint) = 0;
        virtual void add_output_constraint(int node_id, float constraint) = 0;
        virtual void add_setup_constraint(int src_domain_id, int sink_domain_id, float constraint) = 0;
        virtual void add_hold_constraint(int src_domain_id, int sink_domain_id, float constraint) = 0;
        virtual void finish_constraints() = 0;

        virtual void start_delay_model() = 0;
        virtual void add_edge_delay(int edge_id, float min_delay, float max_delay) = 0;
        virtual void finish_delay_model() = 0;

        virtual void start_results() = 0;
        virtual void add_tag(TagType type, int node_id, int domain_id, float arr, float req) = 0;
        virtual void finish_results() = 0;

        //End of parsing
        virtual void finish_parse() = 0;

        //Error during parsing
        virtual void parse_error(const int curr_lineno, const std::string& near_text, const std::string& msg) = 0;
};


/*
 * External functions for loading an SDC file
 */
void tatum_parse_filename(std::string filename, Callback& callback);
void tatum_parse_filename(const char* filename, Callback& callback);

//Loads from 'tatum'. 'filename' only used to pass a filename to callback and can be left unspecified
void tatum_parse_file(FILE* tatum, Callback& callback, const char* filename=""); 

/*
 * Enumerations
 */

} //namespace

#endif