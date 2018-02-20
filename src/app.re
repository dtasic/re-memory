/* get styles file */

[%bs.raw {|require('./app.css')|}];

/* data source and data types */

type tile = {
  id: int,
  sport: string,
  visible: bool,
  solved: bool
};

type tiles = list(tile);

let rawtiles = [
  {id: 1, sport: "baseball", visible: false, solved: false},
  {id: 2, sport: "basketball", visible: false, solved: false},
  {id: 3, sport: "bowling", visible: false, solved: false},
  {id: 4, sport: "football", visible: false, solved: false},
  {id: 5, sport: "hockey", visible: false, solved: false},
  {id: 6, sport: "pool", visible: false, solved: false},
  {id: 7, sport: "rugby", visible: false, solved: false},
  {id: 8, sport: "tennis", visible: false, solved: false},
  {id: 9, sport: "baseball", visible: false, solved: false},
  {id: 10, sport: "basketball", visible: false, solved: false},
  {id: 11, sport: "bowling", visible: false, solved: false},
  {id: 12, sport: "football", visible: false, solved: false},
  {id: 13, sport: "hockey", visible: false, solved: false},
  {id: 14, sport: "pool", visible: false, solved: false},
  {id: 15, sport: "rugby", visible: false, solved: false},
  {id: 16, sport: "tennis", visible: false, solved: false}
];

/* costants, helper functions */

let time_delay = 750;

let str = ReasonReact.stringToElement;

let knuth_shuffle = (a) => {
  let n = Array.length(a);
  let a = Array.copy(a);
  Random.init(int_of_float(Js.Date.now()));
  for (i in n - 1 downto 1) {
    let k = Random.int(i + 1);
    let x = a[k];
    a[k] = a[i];
    a[i] = x
  };
  a
};

let shuffleTiles = (tiles) => Array.of_list(tiles) |> knuth_shuffle |> Array.to_list;

let getOpenedTileId = (tile) => 
  switch (tile) {
    | Some(tile) => tile.id
    | None => 0
  };

let getOpenedTileSport = (tile) =>
  switch (tile) {
    | Some(tile) => tile.sport
    | None => "none"
};

let isAllSolved = (tiles) => List.for_all(tile => tile.solved === true, tiles);

/* state and actions */

type state = {
  tiles: tiles,
  openedtile: option(tile),
  closing: bool,
  attempts: int,
  finished: bool
};

type action = 
  | OpenTile(tile)
  | CheckTiles(tile)
  | CloseTiles
  | ResetGame;

/* single Tile */

module Tile = {
  let component = ReasonReact.statelessComponent("Tile");
  let make = (~tile, ~closing, ~onClick, _children) => {
    ...component,
    render: (_self) =>
      <div className=(
          "tile " 
          ++ (tile.visible || tile.solved ? tile.sport : "") 
          ++ (tile.visible ? " visible" : "") 
          ++ (tile.solved ? " solved" : "")
        ) 
        onClick=(_e => (!tile.solved && !tile.visible && !closing) ? onClick(tile) : ()
      )>
        (str(tile.visible || tile.solved ? "" : "?"))
      </div>
  };
};

/* main App */

let component = ReasonReact.reducerComponent("App");

let make = (_children) => {
  ...component,
  initialState: () => {
    tiles: shuffleTiles(rawtiles),
    openedtile: None,
    closing: false,
    attempts: 0,
    finished: false
  },
  reducer: (action, state) =>
    switch action {
    | ResetGame => 
      let tiles = List.map(tile => {...tile, visible: false, solved: false }, state.tiles);
      ReasonReact.Update({...state, closing: false, attempts: 0, finished: false, tiles: shuffleTiles(tiles)})
    | CloseTiles => 
      let tiles = List.map(tile => {...tile, visible: false }, state.tiles);
      ReasonReact.Update({ tiles: tiles, closing: false, finished: isAllSolved(tiles), attempts: (state.attempts + 1), openedtile: None })
    | CheckTiles(currenttile) => 
      if (getOpenedTileSport(state.openedtile) === currenttile.sport) {
        let tiles = 
          List.map(
            tile => 
            tile.id === getOpenedTileId(state.openedtile) || tile.id === currenttile.id ? 
              {...tile, solved: true } : tile,
            state.tiles
          );
        ReasonReact.Update({ tiles: tiles, closing: false, finished: isAllSolved(tiles), attempts: (state.attempts + 1), openedtile: None });
      } else {
        ReasonReact.SideEffects(self => ignore(Js.Global.setTimeout(() => self.send(CloseTiles), time_delay)));
      };
    | OpenTile(currenttile) => 
      let tiles =
        List.map(
          tile =>
          tile.id === currenttile.id ?
              {...tile, visible: true } : tile,
          state.tiles
        );
      if (getOpenedTileId(state.openedtile) !== 0) {
        ReasonReact.UpdateWithSideEffects({ ...state, tiles: tiles, closing: true }, (self) => self.send(CheckTiles(currenttile)));
      } else {
        ReasonReact.Update({ ...state, tiles: tiles, openedtile: Some(currenttile) })
      }
      
    },
  render: (self) => {
    let finished = 
      switch (self.state.finished) {
      | true => (str("Game is finished!"))
      | false => (str(""))
      };
    <div className="app">
    <div className="header">
        <span className="left">(str("Re-memory - Reason-React memory game"))</span>
        <span className="right">(finished)</span>
      </div>
      <div className="tiles">
        (
          ReasonReact.arrayToElement(Array.of_list(List.map(
            (tile) =>
            <Tile
                key=(string_of_int(tile.id))
                closing=(self.state.closing)
                onClick=(_e => self.send(OpenTile(tile)))
                tile
              />,
              self.state.tiles
          )))
        )
      </div>
      <div className="footer">
        <span className="left">(str("Attempts: " ++ string_of_int(self.state.attempts)))</span>
        <span className="right">
          <button onClick=(_e => self.send(ResetGame))>(str("reset"))</button>
        </span>
      </div>
    </div>
  }
};
