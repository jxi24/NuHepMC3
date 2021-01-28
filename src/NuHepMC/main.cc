#include "HepMC3/GenEvent.h"
#include "HepMC3/WriterAscii.h"
#include "HepMC3/Print.h"

#include <iostream>
using namespace HepMC3;

int main() {
    // Setup generator information
    std::shared_ptr<GenRunInfo> run = std::make_shared<GenRunInfo>();
    // Information about the generator name and version that was used to generate the events
    struct GenRunInfo::ToolInfo generator={std::string("NuGenerator"),std::string("Beta Version"),std::string("Used generator")};
    run->tools().push_back(generator);
    // Load all information about run card used to generate the event into the header.
    struct GenRunInfo::ToolInfo config1={std::string("run_card_name.txt"),std::string("Run Card")};
    struct GenRunInfo::ToolInfo config2={std::string("Experiment Name"),std::string("DUNE")};
    struct GenRunInfo::ToolInfo config3={std::string("Option 1"),std::string("Value")};
    struct GenRunInfo::ToolInfo config4={std::string("Option 2"),std::string("Value")};
    struct GenRunInfo::ToolInfo config5={std::string("Option N"),std::string("Value")};
    run->tools().push_back(config1);
    run->tools().push_back(config2);
    run->tools().push_back(config3);
    run->tools().push_back(config4);
    run->tools().push_back(config5);
    // Set of event weight names to be used for the different weights
    run->set_weight_names({"Default"});

    // Setup the output file
    WriterAscii file("test.hepmc", run);

    // Setup the units to use for the event, and load the run information into event
    GenEvent evt(Units::MEV, Units::MM);
    evt.set_run_info(run);

    // Interaction type
    // List interaction types as integers. Come up with a community standard for the definitons?
    evt.add_attribute("InteractionType", std::make_shared<IntAttribute>(1));

    // Cross Section
    std::shared_ptr<GenCrossSection> cross_section = std::make_shared<GenCrossSection>();
    evt.add_attribute("GenCrossSection", cross_section);
    cross_section->set_cross_section(123.0, 4.5);

    // Event weight
    evt.weight("Default")=123.0;

    // Event position
    // TODO: Do we need more than just the global position of the hard interaction? I.E. should this event be before or after detector simulation?
    FourVector position{123, 45, 6.7, 0.0};
    evt.shift_position_to(position);

    // Dummy event taken from example to have something to print out
    GenParticlePtr p1 = make_shared<GenParticle>( FourVector( 0.0,    0.0,   7000.0,  7000.0  ),2212,  3 );
    GenParticlePtr p2 = make_shared<GenParticle>( FourVector( 0.750, -1.569,   32.191,  32.238),   1,  3 );
    GenParticlePtr p3 = make_shared<GenParticle>( FourVector( 0.0,    0.0,  -7000.0,  7000.0  ),2212,  3 );
    GenParticlePtr p4 = make_shared<GenParticle>( FourVector(-3.047,-19.0,    -54.629,  57.920),  -2,  3 );

    GenVertexPtr v1 = make_shared<GenVertex>();
    v1->add_particle_in (p1);
    v1->add_particle_out(p2);
    evt.add_vertex(v1);

    // Set vertex status if needed
    v1->set_status(4);

    GenVertexPtr v2 = make_shared<GenVertex>();
    v2->add_particle_in (p3);
    v2->add_particle_out(p4);
    evt.add_vertex(v2);

    GenVertexPtr v3 = make_shared<GenVertex>();
    v3->add_particle_in(p2);
    v3->add_particle_in(p4);
    evt.add_vertex(v3);

    GenParticlePtr p5 = make_shared<GenParticle>( FourVector(-3.813,  0.113, -1.833, 4.233),  22, 1 );
    GenParticlePtr p6 = make_shared<GenParticle>( FourVector( 1.517,-20.68, -20.605,85.925), -24, 3 );

    v3->add_particle_out(p5);
    v3->add_particle_out(p6);

    GenVertexPtr v4 = make_shared<GenVertex>();
    v4->add_particle_in (p6);
    evt.add_vertex(v4);

    GenParticlePtr p7 = make_shared<GenParticle>( FourVector(-2.445, 28.816,  6.082,29.552),  1, 1 );
    GenParticlePtr p8 = make_shared<GenParticle>( FourVector( 3.962,-49.498,-26.687,56.373), -2, 1 );

    v4->add_particle_out(p7);
    v4->add_particle_out(p8);

    file.write_event(evt);
    file.close();
}
