#pragma once

#define MAX_PLAYERS 8
#define MAX_SHIPS 500
#define MAX_COLONIES 250
#define MAX_PLANETS 360
#define MAX_STARS 72
#define MAX_LEADERS 67
#define MAX_NEBULAS 5

enum TECH_FIELD {
    TECH_FIELD_STARTING_TECH = 0,  // Starting Tech
    TECH_FIELD_ADVANCED_BIOLOGY = 1,  // Advanced Biology
    TECH_FIELD_ADVANCED_CHEMISTRY = 2,  // Advanced Chemistry
    TECH_FIELD_ADVANCED_CONSTRUCTION = 3,  // Advanced Construction
    TECH_FIELD_ADVANCED_ENGINEERING = 4,  // Advanced Engineering
    TECH_FIELD_ADVANCED_FUSION = 5,  // Advanced Fusion
    TECH_FIELD_ADVANCED_GOVERNMENTS = 6,  // Advanced Governments
    TECH_FIELD_ADVANCED_MAGNETISM = 7,  // Advanced Magnetism
    TECH_FIELD_ADVANCED_MANUFACTURING = 8,  // Advanced Manufacturing
    TECH_FIELD_ADVANCED_METALLURGY = 9,  // Advanced Metallurgy
    TECH_FIELD_MILITARY_TACTICS = 10,  // Military Tactics
    TECH_FIELD_ADVANCED_ROBOTICS = 11,  // Advanced Robotics
    TECH_FIELD_TEACHING_METHODS = 12,  // Teaching Methods
    TECH_FIELD_ANTI_MATTER_FISSION = 13,  // Anti-Matter Fission
    TECH_FIELD_ARTIFICIAL_CONSCIOUSNESS = 14,  // Artificial Consciousness
    TECH_FIELD_ARTIFICIAL_INTELLIGENCE = 15,  // Artificial Intelligence
    TECH_FIELD_ARTIFICIAL_GRAVITY = 16,  // Artificial Gravity
    TECH_FIELD_ARTIFICIAL_LIFE = 17,  // Artificial Life
    TECH_FIELD_ASTRO_BIOLOGY = 18,  // Astro Biology
    TECH_FIELD_ASTRO_CONSTRUCTION = 19,  // Astro Construction
    TECH_FIELD_ASTRO_ENGINEERING = 20,  // Astro Engineering
    TECH_FIELD_CAPSULE_CONSTRUCTION = 21,  // Capsule Construction
    TECH_FIELD_CHEMISTRY = 22,  // Chemistry
    TECH_FIELD_COLD_FUSION = 23,  // Cold Fusion
    TECH_FIELD_CYBERTECHNICS = 24,  // Cybertechnics
    TECH_FIELD_CYBERTRONICS = 25,  // Cybertronics
    TECH_FIELD_DISTORTION_FIELDS = 26,  // Distortion Fields
    TECH_FIELD_ELECTROMAGNETIC_REFRACTION = 27,  // Electromagnetic Refraction
    TECH_FIELD_ELECTRONICS = 28,  // Electronics
    TECH_FIELD_ENGINEERING = 29,  // Engineering
    TECH_FIELD_EVOLUTIONARY_GENETICS = 30,  // Evolutionary Genetics
    TECH_FIELD_FUSION_PHYSICS = 31,  // Fusion Physics
    TECH_FIELD_GALACTIC_ECONOMICS = 32,  // Galactic Economics
    TECH_FIELD_GALACTIC_NETWORKING = 33,  // Galactic Networking
    TECH_FIELD_GENETIC_ENGINEERING = 34,  // Genetic Engineering
    TECH_FIELD_GENETIC_MUTATIONS = 35,  // Genetic Mutations
    TECH_FIELD_GRAVITIC_FIELDS = 36,  // Gravitic Fields
    TECH_FIELD_HIGH_ENERGY_DISTRIBUTION = 37,  // High Energy Distribution
    TECH_FIELD_HYPER_DIMENSIONAL_FISSION = 38,  // Hyper Dimensional Fission
    TECH_FIELD_HYPER_DIMENSIONAL_PHYSICS = 39,  // Hyper Dimensional Physics
    TECH_FIELD_INTERPHASED_FISSION = 40,  // Interphased Fission
    TECH_FIELD_ION_FISSION = 41,  // Ion Fission
    TECH_FIELD_SUPERSCALAR_CONSTRUCTION = 42,  // Superscalar Construction
    TECH_FIELD_MACRO_ECONOMICS = 43,  // Macro Economics
    TECH_FIELD_MACRO_GENETICS = 44,  // Macro Genetics
    TECH_FIELD_MAGNETO_GRAVITICS = 45,  // Magneto Gravitics
    TECH_FIELD_MATTER_ENERGY_CONVERSION = 46,  // Matter Energy Conversion
    TECH_FIELD_MOLECULAR_COMPRESSION = 47,  // Molecular Compression
    TECH_FIELD_MOLECULAR_CONTROL = 48,  // Molecular Control
    TECH_FIELD_MOLECULATRONICS = 49,  // Moleculatronics
    TECH_FIELD_MOLECULAR_MANIPULATION = 50,  // Molecular Manipulation
    TECH_FIELD_MULTI_DIMENSIONAL_PHYSICS = 51,  // Multi-Dimensional Physics
    TECH_FIELD_MULTI_PHASED_PHYSICS = 52,  // Multi-Phased Physics
    TECH_FIELD_NANO_TECHNOLOGY = 53,  // Nano Technology
    TECH_FIELD_NEUTRINO_PHYSICS = 54,  // Neutrino Physics
    TECH_FIELD_NUCLEAR_FISSION = 55,  // Nuclear Fission
    TECH_FIELD_OPTRONICS = 56,  // Optronics
    TECH_FIELD_PHYSICS = 57,  // Physics
    TECH_FIELD_PLANETOID_CONSTRUCTION = 58,  // Planetoid Construction
    TECH_FIELD_PLASMA_PHYSICS = 59,  // Plasma Physics
    TECH_FIELD_POSITRONICS = 60,  // Positronics
    TECH_FIELD_QUANTUM_FIELDS = 61,  // Quantum Fields
    TECH_FIELD_ROBOTICS = 62,  // Robotics
    TECH_FIELD_SERVO_MECHANICS = 63,  // Servo Mechanics
    TECH_FIELD_SUBSPACE_FIELDS = 64,  // Subspace Fields
    TECH_FIELD_SUBSPACE_PHYSICS = 65,  // Subspace Physics
    TECH_FIELD_TACHYON_PHYSICS = 66,  // Tachyon Physics
    TECH_FIELD_TECTONIC_ENGINEERING = 67,  // Tectonic Engineering
    TECH_FIELD_TEMPORAL_FIELDS = 68,  // Temporal Fields
    TECH_FIELD_TEMPORAL_PHYSICS = 69,  // Temporal Physics
    TECH_FIELD__GENETICS = 70,  // Trans Genetics
    TECH_FIELD_TRANSWARP_FIELDS = 71,  // Transwarp Fields
    TECH_FIELD_WARP_FIELDS = 72,  // Warp Fields
    TECH_FIELD_XENO_RELATIONS = 73,  // Xeno Relations
    TECH_FIELD_XENON_TECHNOLOGY = 74,  // Xenon Technology
    TECH_FIELD_BIOLOGY = 75,  // Biology
    TECH_FIELD_POWER = 76,  // Power
    TECH_FIELD_PHYSICS_2 = 77,  // Physics
    TECH_FIELD_CONSTRUCTION = 78,  // Construction
    TECH_FIELD_FIELDS = 79,  // Fields
    TECH_FIELD_CHEMISTRY_2 = 80,  // Chemistry
    TECH_FIELD_COMPUTERS = 81,  // Computers
    TECH_FIELD_SOCIOLOGY = 82,  // Sociology
    TECH_FIELD_COUNT = 83
};

enum TECH_APP {
    TECH_APP_NO_TECH = 0,  // No Tech
    TECH_APP_ACHILLES_TARGETING_UNIT = 1,  // Achilles Targeting Unit
    TECH_APP_ADAMANTIUM_ARMOR = 2,  // Adamantium Armor
    TECH_APP_ADVANCED_CITY_PLANNING = 3,  // Advanced City Planning
    TECH_APP_ADVANCED_DAMAGE_CONTROL = 4,  // Advanced Damage Control
    TECH_APP_ALIEN_MANAGEMENT_CENTER = 5,  // Alien Management Center
    TECH_APP_ANDROID_FARMERS = 6,  // Android Farmers
    TECH_APP_ANDROID_SCIENTISTS = 7,  // Android Scientists
    TECH_APP_ANDROID_WORKERS = 8,  // Android Workers
    TECH_APP_ANTI_GRAV_HARNESS = 9,  // Anti-Grav Harness
    TECH_APP_ANTI_MATTER_BOMB = 10,  // Anti-Matter Bomb
    TECH_APP_ANTI_MATTER_DRIVE = 11,  // Anti-Matter Drive
    TECH_APP_ANTI_MATTER_TORPEDOES = 12,  // Anti-Matter Torpedoes
    TECH_APP_ANTI_MISSILE_ROCKETS = 13,  // Anti-Missile Rockets
    TECH_APP_ARMOR_BARRACKS = 14,  // Armor Barracks
    TECH_APP_ARTEMIS_SYSTEM_NET = 15,  // Artemis System Net
    TECH_APP_PLANET_CONSTRUCTION = 16,  // Planet Construction
    TECH_APP_ASSAULT_SHUTTLES = 17,  // Assault Shuttles
    TECH_APP_ASTRO_UNIVERSITY = 18,  // Astro University
    TECH_APP_ATMOSPHERIC_RENEWER = 19,  // Atmospheric Renewer
    TECH_APP_AUGMENTED_ENGINES = 20,  // Augmented Engines
    TECH_APP_AUTOLAB = 21,  // Autolab
    TECH_APP_AUTOMATED_FACTORIES = 22,  // Automated Factories
    TECH_APP_AUTOMATED_REPAIR_UNIT = 23,  // Automated Repair Unit
    TECH_APP_BATTLEOIDS = 24,  // Battleoids
    TECH_APP_BATTLE_PODS = 25,  // Battle Pods
    TECH_APP_BATTLE_SCANNER = 26,  // Battle Scanner
    TECH_APP_BATTLESTATION = 27,  // Battlestation
    TECH_APP_BIO_TERMINATOR = 28,  // Bio-Terminator
    TECH_APP_BIOMORPHIC_FUNGI = 29,  // Biomorphic Fungi
    TECH_APP_BLACK_HOLE_GENERATOR = 30,  // Black Hole Generator
    TECH_APP_BOMBER_BAYS = 31,  // Bomber Bays
    TECH_APP_CAPITOL = 32,  // Capitol
    TECH_APP_CLASS_I_SHIELD = 33,  // Class I Shield
    TECH_APP_CLASS_III_SHIELD = 34,  // Class III Shield
    TECH_APP_CLASS_V_SHIELD = 35,  // Class V Shield
    TECH_APP_CLASS_VII_SHIELD = 36,  // Class VII Shield
    TECH_APP_CLASS_X_SHIELD = 37,  // Class X Shield
    TECH_APP_CLOAKING_DEVICE = 38,  // Cloaking Device
    TECH_APP_CLONING_CENTER = 39,  // Cloning Center
    TECH_APP_COLONY_BASE = 40,  // Colony Base
    TECH_APP_COLONY_SHIP = 41,  // Colony Ship
    TECH_APP_CONFEDERATION = 42,  // Confederation
    TECH_APP_CYBER_SECURITY_LINK = 43,  // Cyber-Security Link
    TECH_APP_CYBERTRONIC_COMPUTER = 44,  // Cybertronic Computer
    TECH_APP_DAMPER_FIELD = 45,  // Damper Field
    TECH_APP_DAUNTLESS_GUIDANCE_SYSTEM = 46,  // Dauntless Guidance System
    TECH_APP_DEATH_RAY = 47,  // Death Ray
    TECH_APP_DEATH_SPORES = 48,  // Death Spores
    TECH_APP_DEEP_CORE_MINING = 49,  // Deep Core Mining
    TECH_APP_CORE_WASTE_DUMPS = 50,  // Core Waste Dumps
    TECH_APP_DEUTERIUM_FUEL_CELLS = 51,  // Deuterium Fuel Cells
    TECH_APP_DIMENSIONAL_PORTAL = 52,  // Dimensional Portal
    TECH_APP_DISPLACEMENT_DEVICE = 53,  // Displacement Device
    TECH_APP_DISRUPTER_CANNON = 54,  // Disrupter Cannon
    TECH_APP_DOOM_STAR_CONSTRUCTION = 55,  // Doom Star Construction
    TECH_APP_REINFORCED_HULL = 56,  // Reinforced Hull
    TECH_APP_ECM_JAMMER = 57,  // Ecm Jammer
    TECH_APP_ELECTRONIC_COMPUTER = 58,  // Electronic Computer
    TECH_APP_EMISSIONS_GUIDANCE_SYSTEM = 59,  // Emissions Guidance System
    TECH_APP_ENERGY_ABSORBER = 60,  // Energy Absorber
    TECH_APP_BIOSPHERES = 61,  // Biospheres
    TECH_APP_EVOLUTIONARY_MUTATION = 62,  // Evolutionary Mutation
    TECH_APP_EXTENDED_FUEL_TANKS = 63,  // Extended Fuel Tanks
    TECH_APP_FAST_MISSILE_RACKS = 64,  // Fast Missile Racks
    TECH_APP_FEDERATION = 65,  // Federation
    TECH_APP_FIGHTER_BAYS = 66,  // Fighter Bays
    TECH_APP_FIGHTER_GARRISON = 67,  // Fighter Garrison
    TECH_APP_FOOD_REPLICATORS = 68,  // Food Replicators
    TECH_APP_FREIGHTERS = 69,  // Freighters
    TECH_APP_FUSION_BEAM = 70,  // Fusion Beam
    TECH_APP_FUSION_BOMB = 71,  // Fusion Bomb
    TECH_APP_FUSION_DRIVE = 72,  // Fusion Drive
    TECH_APP_FUSION_RIFLE = 73,  // Fusion Rifle
    TECH_APP_GAIA_TRANSFORMATION = 74,  // Gaia Transformation
    TECH_APP_GALACTIC_CURRENCY_EXCHANGE = 75,  // Galactic Currency Exchange
    TECH_APP_GALACTIC_CYBERNET = 76,  // Galactic Cybernet
    TECH_APP_GALACTIC_UNIFICATION = 77,  // Galactic Unification
    TECH_APP_GAUSS_CANNON = 78,  // Gauss Cannon
    TECH_APP_GRAVITON_BEAM = 79,  // Graviton Beam
    TECH_APP_GYRO_DESTABILIZER = 80,  // Gyro Destabilizer
    TECH_APP_HARD_SHIELDS = 81,  // Hard Shields
    TECH_APP_HEAVY_ARMOR = 82,  // Heavy Armor
    TECH_APP_HEAVY_FIGHTER_BAYS = 83,  // Heavy Fighter Bays
    TECH_APP_HEIGHTENED_INTELLIGENCE = 84,  // Heightened Intelligence
    TECH_APP_HIGH_ENERGY_FOCUS = 85,  // High Energy Focus
    TECH_APP_HOLO_SIMULATOR = 86,  // Holo Simulator
    TECH_APP_HYDROPONIC_FARM = 87,  // Hydroponic Farm
    TECH_APP_HYPER_DRIVE = 88,  // Hyper Drive
    TECH_APP_MEGAFLUXERS = 89,  // Megafluxers
    TECH_APP_HYPER_X_CAPACITORS = 90,  // Hyper-X Capacitors
    TECH_APP_HYPERSPACE_COMMUNICATIONS = 91,  // Hyperspace Communications
    TECH_APP_IMPERIUM = 92,  // Imperium
    TECH_APP_INERTIAL_NULLIFIER = 93,  // Inertial Nullifier
    TECH_APP_INERTIAL_STABILIZER = 94,  // Inertial Stabilizer
    TECH_APP_INTERPHASED_DRIVE = 95,  // Interphased Drive
    TECH_APP_ION_DRIVE = 96,  // Ion Drive
    TECH_APP_ION_PULSE_CANNON = 97,  // Ion Pulse Cannon
    TECH_APP_IRIDIUM_FUEL_CELLS = 98,  // Iridium Fuel Cells
    TECH_APP_JUMP_GATE = 99,  // Jump Gate
    TECH_APP_LASER_CANNON = 100,  // Laser Cannon
    TECH_APP_LASER_RIFLE = 101,  // Laser Rifle
    TECH_APP_LIGHTNING_FIELD = 102,  // Lightning Field
    TECH_APP_MARINE_BARRACKS = 103,  // Marine Barracks
    TECH_APP_MASS_DRIVER = 104,  // Mass Driver
    TECH_APP_MAULER_DEVICE = 105,  // Mauler Device
    TECH_APP_MERCULITE_MISSILE = 106,  // Merculite Missile
    TECH_APP_MICROBIOTICS = 107,  // Microbiotics
    TECH_APP_MICROLITE_CONSTRUCTION = 108,  // Microlite Construction
    TECH_APP_OUTPOST_SHIP = 109,  // Outpost Ship
    TECH_APP_MOLECULARTRONIC_COMPUTER = 110,  // Moleculartronic Computer
    TECH_APP_MULTI_WAVE_ECM_JAMMER = 111,  // Multi-Wave Ecm Jammer
    TECH_APP_MULTI_PHASED_SHIELDS = 112,  // Multi-Phased Shields
    TECH_APP_NANO_DISASSEMBLERS = 113,  // Nano Disassemblers
    TECH_APP_NEURAL_SCANNER = 114,  // Neural Scanner
    TECH_APP_NEUTRON_BLASTER = 115,  // Neutron Blaster
    TECH_APP_NEUTRON_SCANNER = 116,  // Neutron Scanner
    TECH_APP_NEUTRONIUM_ARMOR = 117,  // Neutronium Armor
    TECH_APP_NEUTRONIUM_BOMB = 118,  // Neutronium Bomb
    TECH_APP_NUCLEAR_BOMB = 119,  // Nuclear Bomb
    TECH_APP_NUCLEAR_DRIVE = 120,  // Nuclear Drive
    TECH_APP_NUCLEAR_MISSILE = 121,  // Nuclear Missile
    TECH_APP_OPTRONIC_COMPUTER = 122,  // Optronic Computer
    TECH_APP_PARTICLE_BEAM = 123,  // Particle Beam
    TECH_APP_PERSONAL_SHIELD = 124,  // Personal Shield
    TECH_APP_PHASE_SHIFTER = 125,  // Phase Shifter
    TECH_APP_PHASING_CLOAK = 126,  // Phasing Cloak
    TECH_APP_PHASOR = 127,  // Phasor
    TECH_APP_PHASOR_RIFLE = 128,  // Phasor Rifle
    TECH_APP_PLANETARY_BARRIER_SHIELD = 129,  // Planetary Barrier Shield
    TECH_APP_PLANETARY_FLUX_SHIELD = 130,  // Planetary Flux Shield
    TECH_APP_PLANETARY_GRAVITY_GENERATOR = 131,  // Planetary Gravity Generator
    TECH_APP_PLANETARY_MISSILE_BASE = 132,  // Planetary Missile Base
    TECH_APP_GROUND_BATTERIES = 133,  // Ground Batteries
    TECH_APP_PLANETARY_RADIATION_SHIELD = 134,  // Planetary Radiation Shield
    TECH_APP_PLANETARY_STOCK_EXCHANGE = 135,  // Planetary Stock Exchange
    TECH_APP_PLANETARY_SUPERCOMPUTER = 136,  // Planetary Supercomputer
    TECH_APP_PLASMA_CANNON = 137,  // Plasma Cannon
    TECH_APP_PLASMA_RIFLE = 138,  // Plasma Rifle
    TECH_APP_PLASMA_TORPEDOES = 139,  // Plasma Torpedoes
    TECH_APP_PLASMA_WEB = 140,  // Plasma Web
    TECH_APP_PLEASURE_DOME = 141,  // Pleasure Dome
    TECH_APP_POLLUTION_PROCESSOR = 142,  // Pollution Processor
    TECH_APP_POSITRONIC_COMPUTER = 143,  // Positronic Computer
    TECH_APP_POWERED_ARMOR = 144,  // Powered Armor
    TECH_APP_PULSE_RIFLE = 145,  // Pulse Rifle
    TECH_APP_PROTON_TORPEDOES = 146,  // Proton Torpedoes
    TECH_APP_PSIONICS = 147,  // Psionics
    TECH_APP_PULSAR = 148,  // Pulsar
    TECH_APP_PULSON_MISSILE = 149,  // Pulson Missile
    TECH_APP_QUANTUM_DETONATOR = 150,  // Quantum Detonator
    TECH_APP_RANGEMASTER_UNIT = 151,  // Rangemaster Unit
    TECH_APP_RECYCLOTRON = 152,  // Recyclotron
    TECH_APP_REFLECTION_FIELD = 153,  // Reflection Field
    TECH_APP_ROBOTIC_FACTORY = 154,  // Robotic Factory
    TECH_APP_RESEARCH_LABORATORY = 155,  // Research Laboratory
    TECH_APP_ROBO_MINERS = 156,  // Robo-Miners
    TECH_APP_SPACE_SCANNER = 157,  // Space Scanner
    TECH_APP_SCOUT_LAB = 158,  // Scout Lab
    TECH_APP_SECURITY_STATIONS = 159,  // Security Stations
    TECH_APP_SENSORS = 160,  // Sensors
    TECH_APP_SHIELD_CAPACITORS = 161,  // Shield Capacitors
    TECH_APP_SOIL_ENRICHMENT = 162,  // Soil Enrichment
    TECH_APP_SPACE_ACADEMY = 163,  // Space Academy
    TECH_APP_SPACEPORT = 164,  // Spaceport
    TECH_APP_SPATIAL_COMPRESSOR = 165,  // Spatial Compressor
    TECH_APP_SPY_NETWORK = 166,  // Spy Network
    TECH_APP_STANDARD_FUEL_CELLS = 167,  // Standard Fuel Cells
    TECH_APP_STAR_BASE = 168,  // Star Base
    TECH_APP_STAR_FORTRESS = 169,  // Star Fortress
    TECH_APP_STAR_GATE = 170,  // Star Gate
    TECH_APP_STASIS_FIELD = 171,  // Stasis Field
    TECH_APP_STEALTH_FIELD = 172,  // Stealth Field
    TECH_APP_STEALTH_SUIT = 173,  // Stealth Suit
    TECH_APP_STELLAR_CONVERTER = 174,  // Stellar Converter
    TECH_APP_STRUCTURAL_ANALYZER = 175,  // Structural Analyzer
    TECH_APP_SUB_SPACE_COMMUNICATIONS = 176,  // Sub-Space Communications
    TECH_APP_SUB_SPACE_TELEPORTER = 177,  // Sub-Space Teleporter
    TECH_APP_SUBTERRANEAN_FARMS = 178,  // Subterranean Farms
    TECH_APP_SURVIVAL_PODS = 179,  // Survival Pods
    TECH_APP_TACHYON_COMMUNICATIONS = 180,  // Tachyon Communications
    TECH_APP_TACHYON_SCANNER = 181,  // Tachyon Scanner
    TECH_APP_TELEPATHIC_TRAINING = 182,  // Telepathic Training
    TECH_APP_TERRAFORMING = 183,  // Terraforming
    TECH_APP_THORIUM_FUEL_CELLS = 184,  // Thorium Fuel Cells
    TECH_APP_TIME_WARP_FACILITATOR = 185,  // Time Warp Facilitator
    TECH_APP_TITAN_CONSTRUCTION = 186,  // Titan Construction
    TECH_APP_TITANIUM_ARMOR = 187,  // Titanium Armor
    TECH_APP_TRACTOR_BEAM = 188,  // Tractor Beam
    TECH_APP_TRANSPORT = 189,  // Transport
    TECH_APP_TRANSPORTERS = 190,  // Transporters
    TECH_APP_TRITANIUM_ARMOR = 191,  // Tritanium Armor
    TECH_APP_TROOP_PODS = 192,  // Troop Pods
    TECH_APP_UNIVERSAL_ANTIDOTE = 193,  // Universal Antidote
    TECH_APP_URRIDIUM_FUEL_CELLS = 194,  // Urridium Fuel Cells
    TECH_APP_VIRTUAL_REALITY_NETWORK = 195,  // Virtual Reality Network
    TECH_APP_WARP_DISSIPATER = 196,  // Warp Dissipater
    TECH_APP_WARP_INTERDICTOR = 197,  // Warp Interdictor
    TECH_APP_WEATHER_CONTROL_SYSTEM = 198,  // Weather Control System
    TECH_APP_WIDE_AREA_JAMMER = 199,  // Wide Area Jammer
    TECH_APP_XENO_PSYCHOLOGY = 200,  // Xeno Psychology
    TECH_APP_XENTRONIUM_ARMOR = 201,  // Xentronium Armor
    TECH_APP_ZEON_MISSILE = 202,  // Zeon Missile
    TECH_APP_ZORTRIUM_ARMOR = 203,  // Zortrium Armor
    TECH_APP_BIOLOGY = 204,  // Biology
    TECH_APP_POWER = 205,  // Power
    TECH_APP_PHYSICS = 206,  // Physics
    TECH_APP_CONSTRUCTION = 207,  // Construction
    TECH_APP_FIELDS = 208,  // Fields
    TECH_APP_CHEMISTRY = 209,  // Chemistry
    TECH_APP_COMPUTERS = 210,  // Computers
    TECH_APP_SOCIOLOGY = 211,  // Sociology
    TECH_APP_COUNT = 212
};

enum BUILDING {
    BUILDING_NO_BUILDING = 0,  // No Building
    BUILDING_ALIEN_CONTROL_CENTER = 1,  // Alien Control Center
    BUILDING_ARMOR_BARRACKS = 2,  // Armor Barracks
    BUILDING_ARTEMIS_SYSTEM_NET = 3,  // Artemis System Net
    BUILDING_ASTRO_UNIVERSITY = 4,  // Astro University
    BUILDING_ATMOSPHERE_RENEWER = 5,  // Atmosphere Renewer
    BUILDING_AUTOLAB = 6,  // Autolab
    BUILDING_AUTOMATED_FACTORY = 7,  // Automated Factory
    BUILDING_BATTLESTATION = 8,  // Battlestation
    BUILDING_CAPITOL = 9,  // Capitol
    BUILDING_CLONING_CENTER = 10,  // Cloning Center
    BUILDING_COLONY_BASE = 11,  // Colony Base
    BUILDING_DEEP_CORE_MINE = 12,  // Deep Core Mine
    BUILDING_CORE_WASTE_DUMP = 13,  // Core Waste Dump
    BUILDING_DIMENSIONAL_PORTAL = 14,  // Dimensional Portal
    BUILDING_BIOSPHERES = 15,  // Biospheres
    BUILDING_FOOD_REPLICATORS = 16,  // Food Replicators
    BUILDING_GAIA_TRANSFORMATION = 17,  // Gaia Transformation
    BUILDING_CURRENCY_EXCHANGE = 18,  // Currency Exchange
    BUILDING_GALACTIC_CYBERNET = 19,  // Galactic Cybernet
    BUILDING_HOLO_SIMULATOR = 20,  // Holo Simulator
    BUILDING_HYDROPONIC_FARM = 21,  // Hydroponic Farm
    BUILDING_MARINE_BARRACKS = 22,  // Marine Barracks
    BUILDING_BARRIER_SHIELD = 23,  // Barrier Shield
    BUILDING_FLUX_SHIELD = 24,  // Flux Shield
    BUILDING_GRAVITY_GENERATOR = 25,  // Gravity Generator
    BUILDING_MISSILE_BASE = 26,  // Missile Base
    BUILDING_GROUND_BATTERIES = 27,  // Ground Batteries
    BUILDING_RADIATION_SHIELD = 28,  // Radiation Shield
    BUILDING_STOCK_EXCHANGE = 29,  // Stock Exchange
    BUILDING_SUPERCOMPUTER = 30,  // Supercomputer
    BUILDING_PLEASURE_DOME = 31,  // Pleasure Dome
    BUILDING_POLLUTION_PROCESSOR = 32,  // Pollution Processor
    BUILDING_RECYCLOTRON = 33,  // Recyclotron
    BUILDING_ROBOTIC_FACTORY = 34,  // Robotic Factory
    BUILDING_RESEARCH_LAB = 35,  // Research Lab
    BUILDING_ROBO_MINER_PLANT = 36,  // Robo Miner Plant
    BUILDING_SOIL_ENRICHMENT = 37,  // Soil Enrichment
    BUILDING_SPACE_ACADEMY = 38,  // Space Academy
    BUILDING_SPACEPORT = 39,  // Spaceport
    BUILDING_STAR_BASE = 40,  // Star Base
    BUILDING_STAR_FORTRESS = 41,  // Star Fortress
    BUILDING_STELLAR_CONVERTER = 42,  // Stellar Converter
    BUILDING_SUBTERRANEAN_FARMS = 43,  // Subterranean Farms
    BUILDING_TERRAFORMING = 44,  // Terraforming
    BUILDING_WARP_INTERDICTOR = 45,  // Warp Interdictor
    BUILDING_WEATHER_CONTROLLER = 46,  // Weather Controller
    BUILDING_FIGHTER_GARRISON = 47,  // Fighter Garrison
    BUILDING_ARTIFICIAL_PLANET = 48,  // Artificial Planet
    BUILDING_COUNT = 49
};

enum SPECIAL {
    SPECIAL_NO_SPECIAL = 0,  // No Special
    SPECIAL_ACHILLES_TARGETING_UNIT = 1,  // Achilles Targeting Unit
    SPECIAL_AUGMENTED_ENGINES = 2,  // Augmented Engines
    SPECIAL_AUTOMATED_REPAIR_UNIT = 3,  // Automated Repair Unit
    SPECIAL_BATTLE_PODS = 4,  // Battle Pods
    SPECIAL_BATTLE_SCANNER = 5,  // Battle Scanner
    SPECIAL_CLOAKING_DEVICE = 6,  // Cloaking Device
    SPECIAL_DAMPER_FIELD = 7,  // Damper Field
    SPECIAL_DISPLACEMENT_DEVICE = 8,  // Displacement Device
    SPECIAL_ECM_JAMMER = 9,  // Ecm Jammer
    SPECIAL_ENERGY_ABSORBER = 10,  // Energy Absorber
    SPECIAL_EXTENDED_FUEL_TANKS = 11,  // Extended Fuel Tanks
    SPECIAL_FAST_MISSILE_RACKS = 12,  // Fast Missile Racks
    SPECIAL_HARD_SHIELDS = 13,  // Hard Shields
    SPECIAL_HEAVY_ARMOR = 14,  // Heavy Armor
    SPECIAL_HIGH_ENERGY_FOCUS = 15,  // High Energy Focus
    SPECIAL_HYPER_X_CAPACITORS = 16,  // Hyper-X Capacitors
    SPECIAL_INERTIAL_NULLIFIER = 17,  // Inertial Nullifier
    SPECIAL_INERTIAL_STABILIZER = 18,  // Inertial Stabilizer
    SPECIAL_LIGHTNING_FIELD = 19,  // Lightning Field
    SPECIAL_MULTI_PHASED_SHIELDS = 20,  // Multi-Phased Shields
    SPECIAL_MULTI_WAVE_ECM_JAMMER = 21,  // Multi-Wave Ecm Jammer
    SPECIAL_PHASE_SHIFTER = 22,  // Phase Shifter
    SPECIAL_PHASING_CLOAK = 23,  // Phasing Cloak
    SPECIAL_QUANTUM_DETONATOR = 24,  // Quantum Detonator
    SPECIAL_RANGEMASTER_UNIT = 25,  // Rangemaster Unit
    SPECIAL_REFLECTION_FIELD = 26,  // Reflection Field
    SPECIAL_REINFORCED_HULL = 27,  // Reinforced Hull
    SPECIAL_SCOUT_LAB = 28,  // Scout Lab
    SPECIAL_SECURITY_STATIONS = 29,  // Security Stations
    SPECIAL_SHIELD_CAPACITOR = 30,  // Shield Capacitor
    SPECIAL_STEALTH_FIELD = 31,  // Stealth Field
    SPECIAL_STRUCTURAL_ANALYZER = 32,  // Structural Analyzer
    SPECIAL_SUB_SPACE_TELEPORTER = 33,  // Sub Space Teleporter
    SPECIAL_TIME_WARP_FACILITATOR = 34,  // Time Warp Facilitator
    SPECIAL_TRANSPORTERS = 35,  // Transporters
    SPECIAL_TROOP_PODS = 36,  // Troop Pods
    SPECIAL_WARP_DISSIPATOR = 37,  // Warp Dissipator
    SPECIAL_WIDE_AREA_JAMMER = 38,  // Wide Area Jammer
    SPECIAL_REGENERATION = 39,  // Regeneration
    SPECIAL_COUNT = 40
};

enum ARMOR {
    ARMOR_NO_ARMOR = 0,  // No Armor
    ARMOR_TITANIUM_ARMOR = 1,  // Titanium Armor
    ARMOR_TRITANIUM_ARMOR = 2,  // Tritanium Armor
    ARMOR_ZORTRIUM_ARMOR = 3,  // Zortrium Armor
    ARMOR_NEUTRONIUM_ARMOR = 4,  // Neutronium Armor
    ARMOR_ADAMANTIUM_ARMOR = 5,  // Adamantium Armor
    ARMOR_XENTRONIUM_ARMOR = 6,  // Xentronium Armor
    ARMOR_COUNT = 7
};

enum SHIELD {
    SHIELD_NO_SHIELD = 0,  // No Shield
    SHIELD_CLASS_I_SHIELD = 1,  // Class I Shield
    SHIELD_CLASS_III_SHIELD = 2,  // Class III Shield
    SHIELD_CLASS_V_SHIELD = 3,  // Class V Shield
    SHIELD_CLASS_VII_SHIELD = 4,  // Class VII Shield
    SHIELD_CLASS_X_SHIELD = 5,  // Class X Shield
    SHIELD_COUNT = 6
};

enum WEAPON {
    WEAPON_NO_WEAPONS = 0,  // No Weapons
    WEAPON_MASS_DRIVER = 1,  // Mass Driver
    WEAPON_GAUSS_CANNON = 2,  // Gauss Cannon
    WEAPON_LASER_CANNON = 3,  // Laser Cannon
    WEAPON_PARTICLE_BEAM = 4,  // Particle Beam
    WEAPON_FUSION_BEAM = 5,  // Fusion Beam
    WEAPON_ION_PULSE_CANNON = 6,  // Ion Pulse Cannon
    WEAPON_GRAVITON_BEAM = 7,  // Graviton Beam
    WEAPON_NEUTRON_BLASTER = 8,  // Neutron Blaster
    WEAPON_PHASOR = 9,  // Phasor
    WEAPON_DISRUPTER = 10,  // Disrupter
    WEAPON_DEATH_RAY = 11,  // Death Ray
    WEAPON_PLASMA_CANNON = 12,  // Plasma Cannon
    WEAPON_SPATIAL_COMPRESSOR = 13,  // Spatial Compressor
    WEAPON_NUCLEAR_MISSILE = 14,  // Nuclear Missile
    WEAPON_MERCULITE_MISSILE = 15,  // Merculite Missile
    WEAPON_PULSON_MISSILE = 16,  // Pulson Missile
    WEAPON_ZEON_MISSILE = 17,  // Zeon Missile
    WEAPON_ANTI_MATTER_TORP = 18,  // Anti-Matter Torp
    WEAPON_PROTON_TORPEDO = 19,  // Proton Torpedo
    WEAPON_PLASMA_TORPEDO = 20,  // Plasma Torpedo
    WEAPON_NUCLEAR_BOMB = 21,  // Nuclear Bomb
    WEAPON_FUSION_BOMB = 22,  // Fusion Bomb
    WEAPON_ANTI_MATTER_BOMB = 23,  // Anti-Matter Bomb
    WEAPON_NEUTRONIUM_BOMB = 24,  // Neutronium Bomb
    WEAPON_DEATH_SPORE = 25,  // Death Spore
    WEAPON_BIO_TERMINATOR = 26,  // Bio-Terminator
    WEAPON_MAULER_DEVICE = 27,  // Mauler Device
    WEAPON_ASSAULT_SHUTTLE = 28,  // Assault Shuttle
    WEAPON_HEAVY_FIGHTER = 29,  // Heavy Fighter
    WEAPON_BOMBER = 30,  // Bomber
    WEAPON_INTERCEPTOR = 31,  // Interceptor
    WEAPON_STASIS_FIELD = 32,  // Stasis Field
    WEAPON_ANTI_MISSILE_ROCKET = 33,  // Anti-Missile Rocket
    WEAPON_GYRO_DESTABILIZER = 34,  // Gyro Destabilizer
    WEAPON_PLASMA_WEB = 35,  // Plasma Web
    WEAPON_PULSAR = 36,  // Pulsar
    WEAPON_BLACK_HOLE_GENERATOR = 37,  // Black Hole Generator
    WEAPON_STELLAR_CONVERTER = 38,  // Stellar Converter
    WEAPON_TRACTOR_BEAM = 39,  // Tractor Beam
    WEAPON_DRAGON_BREATH = 40,  // Dragon Breath
    WEAPON_PHASOR_EYE = 41,  // Phasor Eye
    WEAPON_CRYSTAL_RAY = 42,  // Crystal Ray
    WEAPON_PLASMA_BREATH = 43,  // Plasma Breath
    WEAPON_PLASMA_FLUX = 44,  // Plasma Flux
    WEAPON_CAUSTIC_SLIME = 45,  // Caustic Slime
    WEAPON_COUNT = 46
};

enum COMPUTER {
    COMPUTER_NO_COMPUTER = 0,  // No Computer
    COMPUTER_ELECTRONIC_COMPUTER = 1,  // Electronic Computer
    COMPUTER_OPTRONIC_COMPUTER = 2,  // Optronic Computer
    COMPUTER_POSITRONIC_COMPUTER = 3,  // Positronic Computer
    COMPUTER_CYBERTRONIC_COMPUTER = 4,  // Cybertronic Computer
    COMPUTER_MOLECULARTRONIC_COMPUTER = 5,  // Moleculartronic Computer
    COMPUTER_COUNT = 6
};

enum DRIVE {
    DRIVE_NO_DRIVE = 0,  // No Drive
    DRIVE_NUCLEAR_DRIVE = 1,  // Nuclear Drive
    DRIVE_FUSION_DRIVE = 2,  // Fusion Drive
    DRIVE_ION_DRIVE = 3,  // Ion Drive
    DRIVE_ANTI_MATTER_DRIVE = 4,  // Anti-Matter Drive
    DRIVE_HYPER_DRIVE = 5,  // Hyper Drive
    DRIVE_INTERPHASED_DRIVE = 6,  // Interphased Drive
    DRIVE_COUNT = 7
};

enum UNIT {
    UNIT_NO_UNIT = 0,  // No Unit
    UNIT_MARINES = 1,  // Marines
    UNIT_ARMOR = 2,  // Armor
    UNIT_SPY = 3,  // Spy
    UNIT_COUNT = 4
};

enum FUEL {
    FUEL_NO_FUEL_CELLS = 0,  // No Fuel Cells
    FUEL_STANDARD_FUEL_CELLS = 1,  // Standard Fuel Cells
    FUEL_DEUTERIUM_FUEL_CELLS = 2,  // Deuterium Fuel Cells
    FUEL_IRIDIUM_FUEL_CELLS = 3,  // Iridium Fuel Cells
    FUEL_URIDIUM_FUEL_CELLS = 4,  // Uridium Fuel Cells
    FUEL_THORIUM_FUEL_CELLS = 5,  // Thorium Fuel Cells
    FUEL_COUNT = 6
};

enum WEAPON_MOD {
    WEAPON_MOD_NO_MODS = 0,  // No Mods
    WEAPON_MOD_HEAVY_MOUNT = 1,  // Heavy Mount
    WEAPON_MOD_POINT_DEFENSE = 2,  // Point Defense
    WEAPON_MOD_ARMOR_PIERCING = 3,  // Armor Piercing
    WEAPON_MOD_CONTINUOUS = 4,  // Continuous
    WEAPON_MOD_NO_RANGE_DISSIPATION = 5,  // No Range Dissipation
    WEAPON_MOD_SHIELD_PIERCING = 6,  // Shield Piercing
    WEAPON_MOD_AUTO_FIRE = 7,  // Auto Fire
    WEAPON_MOD_ENVELOPING = 8,  // Enveloping
    WEAPON_MOD_MIRV = 9,  // Mirv
    WEAPON_MOD_ECCM = 10,  // ECCM
    WEAPON_MOD_HEAVILY_ARMORED = 11,  // Heavily Armored
    WEAPON_MOD_FAST = 12,  // Fast
    WEAPON_MOD_EMMISIONS_GUIDANCE = 13,  // Emmisions Guidance
    WEAPON_MOD_OVERLOADED = 14,  // Overloaded
    WEAPON_MOD_COUNT = 15
};

enum HULL_CLASS {
    HULL_FRIGATE = 0,  // Frigate
    HULL_DESTROYER = 1,  // Destroyer
    HULL_CRUISER = 2,  // Cruiser
    HULL_BATTLESHIP = 3,  // Battleship
    HULL_TITAN = 4,  // Titan
    HULL_DOOM_STAR = 5,  // Doom Star
    HULL_STAR_BASE = 6,  // Star Base
    HULL_BATTLESTATION = 7,  // Battlestation
    HULL_STAR_FORTRESS = 8,  // Star Fortress
    HULL_CLASS_COUNT = 9
};

enum TRAIT {
    TRAIT_CURRENT_GOVERNMENT = 0,
    TRAIT_POP_GROWTH_BONUS,
    TRAIT_FOOD_BONUS,
    TRAIT_PRODUCTION_BONUS,
    TRAIT_RESEARCH_BONUS,
    TRAIT_TAX_BONUS,
    TRAIT_SHIP_DEFENSE,
    TRAIT_SHIP_ATTACK,
    TRAIT_GROUND_COMBAT_BONUS,
    TRAIT_SPYING_BONUS,
    TRAIT_LOW_G_WORLD,
    TRAIT_HEAVY_G_WORLD,
    TRAIT_AQUATIC,
    TRAIT_SUBTERRANEAN,
    TRAIT_LARGE_HOME_WORLD,
    TRAIT_RICH_HOME_WORLD,
    TRAIT_ANCIENT_HOME_WORLD,
    TRAIT_CYBERNETIC,
    TRAIT_EATS_MINERALS,
    TRAIT_REPULSIVE,
    TRAIT_CHARISMATIC,
    TRAIT_UNCREATIVE,
    TRAIT_CREATIVE,
    TRAIT_ENVIRONMENT_IMMUNE,
    TRAIT_FANTASTIC_TRADERS,
    TRAIT_TELEPATHIC,
    TRAIT_LUCKY,
    TRAIT_OMNISCIENCE,
    TRAIT_STEALTHY_SHIPS,
    TRAIT_TRANS_DIMENSIONAL,
    TRAIT_WARLORD,

    TRAIT_COUNT
};
