#include "tile_type_manager.h"

tile_type_manager::tile_type_manager(){
  //ctor
}

tile_type_manager::~tile_type_manager(){
  //dtor
}

// Load tiles
void tile_type_manager::load( std::string newFile){
  // Load biomes from file
  rapidxml::xml_document<> doc;
  std::ifstream file;

  // Check exist
  if( fexists(newFile.c_str())){
    file.open(newFile.c_str());
  }
  else{
    abort_on_error( std::string("Cannot find file " + newFile + " \n Please check your files and try again").c_str());
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string content(buffer.str());
  doc.parse<0>(&content[0]);

  rapidxml::xml_node<> *allTiles = doc.first_node();

  // Loading
  std::cout << "   TILES\n-------------\n";

  // Load tiles
  for(rapidxml::xml_node<> *cTile=allTiles-> first_node("tile"); cTile; cTile=cTile->next_sibling()){
    // Read xml variables
    // General
    int tileID = atoi(cTile-> first_attribute("id") -> value());
    std::string name = cTile-> first_node("name") -> value();
    std::string image1 = cTile-> first_node("images") -> first_node("image") -> value();
    std::string image2 = cTile-> first_node("images") -> first_node("image2") -> value();
    std::string model = cTile-> first_node("model") -> value();
    int randomness = atoi(cTile-> first_node("random") -> value());

    std::string attrubite_string = cTile-> first_node("attrubite") -> value();
    int attrubite = ATTRIBUTE_GAS;

    // Get attrubite
    if( attrubite_string == "ATTRIBUTE_GAS")
      attrubite = ATTRIBUTE_GAS;
    else if( attrubite_string == "ATTRIBUTE_SOLID")
      attrubite = ATTRIBUTE_SOLID;
    else if( attrubite_string == "ATTRIBUTE_LIQUID")
      attrubite = ATTRIBUTE_LIQUID;

    // Draw to screen (debug)
    std::cout << "-> Loading Tile:" << name << "  ID:" <<  tileID << "  MODEL:" << model << "  ATTRIBUTE:" << attrubite_string << "  RANDOMNESS:" << randomness << "\n";

    // Create tile, set variables and add it to the tile list
    tile_type newTileType( name, tileID, NULL, model, attrubite, randomness);

    // Set images
    newTileType.setImages( image1, image2);

    // Add the tile
    tile_defs.push_back( newTileType);
  }

  std::cout << "\n\n";
}

tile_type *tile_type_manager::getTileByType( int tileID){
  return &tile_defs.at( tileID);
}
