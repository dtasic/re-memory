/* costants, helper functions */

let str = ReasonReact.stringToElement;

let time_delay = 750;

let timer_update = 10;

let knuth_shuffle = a => {
  let n = Array.length(a);
  let a = Array.copy(a);
  
  Js.Date.now() |> int_of_float |> Random.init;

  for (i in n - 1 downto 1) {
    let k = Random.int(i + 1);
    let x = a[k];
    a[k] = a[i];
    a[i] = x;
  };
  a;
};

let shuffleTiles = tiles =>
  Array.of_list(tiles) |> knuth_shuffle |> Array.to_list;

let getOpenedTileId = (tile: option(Data.tile)) =>
  switch tile {
  | Some(tile) => tile.id
  | None => 0
  };

let getOpenedTileSport = (tile: option(Data.tile)) =>
  switch tile {
  | Some(tile) => tile.sport
  | None => "none"
  };

let isAllSolved = tiles =>
  List.for_all((tile: Data.tile) => tile.solved === true, tiles);