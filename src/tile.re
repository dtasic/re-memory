/* single Tile */

let component = ReasonReact.statelessComponent("Tile");
let make = (~tile: Data.tile, ~closing, ~onClick, _children) => {
  ...component,
  render: (_self) =>
    <div className=(
        "tile " 
        ++ (tile.status === Visible || tile.status === Solved ? tile.sport : "") 
        ++ (tile.status === Visible ? " visible" : "") 
        ++ (tile.status === Solved ? " solved" : "")
      ) 
      onClick=(_e => (tile.status !== Visible && tile.status !== Solved && !closing) ? onClick(tile) : ()
    )>
      (Utils.str(tile.status === Visible || tile.status === Solved ? "" : "?"))
    </div>
};
