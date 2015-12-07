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

  // Load tiles
  for(rapidxml::xml_node<> *cTile=allTiles-> first_node("tile"); cTile; cTile=cTile->next_sibling()){
    // Read xml variables
    // General
    int tileID = atoi(cTile-> first_attribute("id") -> value());
    std::string name = cTile-> first_node("name") -> value();
    std::string image = cTile-> first_node("images") -> first_node("image") -> value();

    // Draw to screen (debug)
    textprintf_centre_ex(screen,font,640,700,makecol(0,0,0),makecol(255,255,255),"Loading Tile:%s ID:%i",name.c_str(), tileID);

    // Create tile, set variables and add it to the tile list
    tile_type newTileType( name, tileID, NULL);

    // Set images
    newTileType.setImages( image, "NULL");

    // Add the tile
    tile_defs.push_back( newTileType);
  }
}

tile_type *tile_type_manager::getTileByType( int tileID){
  return &tile_defs.at( tileID);
}